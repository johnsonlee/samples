{
    'variables' : {
        'pkg-config' : 'pkg-config',
    }, 'targets' : [{
        'target_name' : 'fbinfo',
        'type'        : 'executable',
        'sources'     : [
            'fbinfo.c',
        ],
    }, {
        'target_name' : 'fb2ppm',
        'type'        : 'executable',
        'sources'     : [
            'fb2ppm.c',
        ],
    }, {
        'target_name' : 'fbviewer',
        'type'        : 'executable',
        'sources'     : [
            'fbviewer.c',
        ],
        'cflags' : [
            '<!@(<(pkg-config) --cflags gtk+-2.0)',
        ],
        'link_settings' : {
            'ldflags' : [
                '<!@(<(pkg-config) --libs-only-L gtk+-2.0)',
            ],
            'libraries' : [
                '<!@(<(pkg-config) --libs-only-l gtk+-2.0)',
            ],
        },
    }],
}
