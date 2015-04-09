var Pointer = require('../').Pointer;

var p = Pointer.alloc();

console.log(JSON.stringify(p));

p.release();
p.release();
p.release();
