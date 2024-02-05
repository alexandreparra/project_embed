#include <iostream>

#include <hl.h>

// hlmodule doesn't have extern "C", from our C++ code this is needed to not cause any name mangling problems.
extern "C" {
#include <hlmodule.h>
}

#ifdef HL_WIN
#include <clocale>
typedef uchar pchar;
#define pprintf(str, file) uprintf(USTR(str), file)
#define pfopen(file, ext) _wfopen(file, USTR(ext))
#define pcompare wcscmp
#define ptoi(s) wcstol(s,NULL,10)
#define PSTR(x) USTR(x)

#else

#include <sys/stat.h>
typedef char pchar;
#define pprintf printf
#define pfopen fopen
#define pcompare strcmp
#define ptoi atoi
#define PSTR(x) x
#endif

struct MainContext {
    const char *file;
    hl_code    *code;
    hl_module  *module;
    vdynamic   *ret;
    int         file_time;
};

// Copied and adapted from https://github.com/alexandreparra/tinyconfig/blob/master/src/tinyconfig.c
static int open_file(FILE **fp, const char *file_path, const char *mode) {
#ifdef HL_WIN
    int err = fopen_s(fp, file_path, mode);
    if (err) return 1;
#else
    *fp = fopen(file_path, mode);
#endif

    if (fgetc(*fp) == EOF || ferror(*fp))
        return 1;

    return 0;
}

static int pfiletime(const char *file) {
#ifdef HL_WIN
    struct _stat32 st{};
    _wstat32((wchar_t *) file, &st);
#else
    struct stat st {};
    stat(file, &st);
#endif
    return static_cast<int> (st.st_mtime);
}

static hl_code *load_code(const char *file_path, char **error_msg, bool print_errors) {
    hl_code *code;
    FILE *f = nullptr;
    int ret = open_file(&f, file_path, "rb");
    if (ret) return nullptr;

    int pos, size;
    char *fdata;
    if (f == nullptr) {
        if (print_errors) std::cout << "File not found" << file_path << '\n';
        return nullptr;
    }

    fseek(f, 0, SEEK_END);
    size = (int) ftell(f);
    fseek(f, 0, SEEK_SET);
    fdata = (char *) malloc(size);
    pos = 0;

    while (pos < size) {
        int r = (int) fread(fdata + pos, 1, size - pos, f);
        if (r <= 0) {
            if (print_errors) std::cout << "Failed to read " << file_path << '\n';
            return nullptr;
        }
        pos += r;
    }

    fclose(f);
    code = hl_code_read((unsigned char *) fdata, size, error_msg);
    free(fdata);
    return code;
}

static bool check_reload(MainContext *ctx) {
    int time = pfiletime(ctx->file);
    bool changed;
    if (time == ctx->file_time)
        return false;

    char *error_msg = nullptr;
    hl_code *code = load_code(ctx->file, &error_msg, false);
    if (code == nullptr)
        return false;

    changed = hl_module_patch(ctx->module, code);
    ctx->file_time = time;
    hl_code_free(code);

    return changed;
}

#ifdef HL_VCC
// this allows some runtime detection to switch to high performance mode
__declspec(dllexport) DWORD NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif

#if defined(HL_LINUX) || defined(HL_MAC)
#include <signal.h>
static void handle_signal( int signum ) {
    signal(signum, SIG_DFL);
    printf("SIGNAL %d\n",signum);
    hl_dump_stack();
    fflush(stdout);
    raise(signum);
}
static void setup_handler() {
    struct sigaction act;
    act.sa_sigaction = NULL;
    act.sa_handler = handle_signal;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    signal(SIGPIPE, SIG_IGN);
    sigaction(SIGSEGV,&act,NULL);
    sigaction(SIGTERM,&act,NULL);
}
#else

static void setup_handler() {
}
#endif

#ifdef HL_WIN
int wmain(int argc, pchar *argv[]) {
#else
    int main(int argc, pchar *argv[]) {
#endif

    // Hashlink originally receives the file_path path to start execution from the system args.
    // As we are embedding Haxe/Hashlink we can optionally provide the path from within our code. 
    // Be aware that this is a relative path and depends from where you are executing the code.
    // In our case, using the compile.ps1 file we'll be executing our binary inside /bin folder,
    // so the current string finds our Hashlink file correctly. Change this according to your setup.
    std::string file_path = "../hx/main.hl";

    static vclosure cl;
    char *error_msg = nullptr;

    // Used to display exception stack trace, set by hl_dyn_call_safe further down.
    bool is_exec = false;
    int first_boot_arg = -1;
    MainContext ctx = {};

    // Flags that are normally set through the CLI, some of them don't have any use for us.
    int debug_port = -1;
    int profile_count = -1;
    bool debug_wait = false;
    bool hot_reload = false;

    hl_global_init();
    hl_sys_init((void **) argv, argc, (void *) file_path.c_str());
    hl_register_thread(&ctx);

    ctx.file = file_path.c_str();
    ctx.code = load_code(file_path.c_str(), &error_msg, true);
    if (ctx.code == nullptr) {
        if (error_msg) std::cout << error_msg << '\n';
        return 1;
    }

    ctx.module = hl_module_alloc(ctx.code);
    if (ctx.module == nullptr)
        return 2;
    if (!hl_module_init(ctx.module, hot_reload))
        return 3;
    if (hot_reload) {
        ctx.file_time = pfiletime(ctx.file);
        hl_setup_reload_check((void *) check_reload, &ctx);
    }

    cl.t = ctx.code->functions[ctx.module->functions_indexes[ctx.module->code->entrypoint]].type;
    cl.fun = ctx.module->functions_ptrs[ctx.module->code->entrypoint];
    cl.hasValue = 0;
    setup_handler();
    hl_profile_setup(profile_count);
    ctx.ret = hl_dyn_call_safe(&cl, nullptr, 0, &is_exec);

    if (is_exec) {
        varray *exceptions = hl_exception_stack();

        uprintf(USTR("Uncaught exception: %s\n"), hl_to_string(ctx.ret));
        for (int i = 0; i < exceptions->size; i++)
            uprintf(USTR("Called from %s\n"), hl_aptr(exceptions, uchar*)[i]);

        hl_debug_break();
        hl_global_free();
        return 1;
    }

    hl_module_free(ctx.module);
    hl_free(&ctx.code->alloc);
    hl_global_free();

    return 0;
}
