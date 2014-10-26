#!/opt/local/libexec/gnubin/env node

var CMD = 'curl http://app.myzaker.com/meitu/next.php?url=http%3A%2F%2Fiphone.myzaker.com%2Fzaker%2Fblog2news.php%3Fapp_id%3D1156%26since_date%3D1412818462%26nt%3D1%26_appid%3D';

var exec = require('child_process').exec;

function print(obj) {
    function _print(o, out, indent) {
        if ('object' === typeof o) {
            if (o instanceof Array) {
                out.write('[\n');

                for (var i = 0; i < o.length; i++) {
                    if (i > 0) {
                        out.write(',\n');
                    }

                    out.write(indent);
                    out.write('    ');
                    _print(o[i], out, indent + '    ');
                }

                out.write('\n');
                out.write(indent);
                out.write(']');
            } else {
                out.write('{');

                var k = 0;
                for (var i in o) {
                    if (k++ > 0) {
                        out.write(',');
                    }

                    out.write('\n');
                    out.write(indent);
                    out.write('    ');
                    out.write(JSON.stringify(i));
                    out.write(' : ');
                    _print(o[i], out, indent + '    ');
                }

                out.write('\n');
                out.write(indent);
                out.write('}');
            }
        } else {
            out.write(JSON.stringify(o));
        }
    }

    _print(obj, process.stdout, '');
}

exec(CMD, function(error, stdout, stderr) {
    if (error != null) {
        console.log(error);
    } else {
        print(JSON.parse(stdout).data);
    }
});
