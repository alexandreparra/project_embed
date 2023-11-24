package ringbuffer;

// A shortcut to reference our C ring_buffer struct.
private typedef RingBufferPtr = hl.Abstract<"ring_buffer">;

// "ring_buffer" is the shared library that Hashlink needs to load.
@:hlNative("ring_buffer")
private class Lib {
    // Here we'll be just returning mock values, as Hashlink will take care of calling the native functions. For functions
    // that return Void, just omit any code.

    public static function alloc(size: Int): RingBufferPtr { return null; }

    public static function write(rb: RingBufferPtr, item: Int): Void { }

    public static function read(rb: RingBufferPtr): Int { return 0; }

    public static function getCapacity(rb: RingBufferPtr): Int { return 0; }
}

// Simple wrapper
class RingBuffer {

    var rb: RingBufferPtr;

    public function new(size: Int) {
        rb = Lib.alloc(size);
    }

    public function write(item: Int) {
        Lib.write(rb, item);
    }

    public function read(): Int {
        return Lib.read(rb);
    }

    public function getCapacity(): Int {
        return Lib.getCapacity(rb);
    }

}
