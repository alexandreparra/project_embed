package;

import ringbuffer.RingBuffer;

class Main {

    static public function main() {
        trace("Haxe: ring buffer test");
        var rb = new RingBuffer(5);
        trace("Haxe: write " + rb.getCapacity());
        rb.write(1);
        rb.write(2);
        trace("Haxe: read " + rb.read());
    }

}