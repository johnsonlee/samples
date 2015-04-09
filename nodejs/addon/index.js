var addon = require('./build/Release/addon.node');
var Pointer = addon.Pointer;

Pointer.alloc = function() {
    return addon.alloc.apply(addon, arguments);
};

Pointer.prototype.release = function() {
    var args = Array.prototype.slice.apply(arguments);
    args.unshift(this);
    return addon.release.apply(addon, args);
};

module.exports.Pointer = Pointer;
