{
    "targets": [
        {
            "target_name": "ovllm",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": [
                "ovllm.cpp",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "./include",
            ],
            "libraries": [
                "..\\lib\\intel64\\Release\\openvino.lib",
                "..\\lib\\intel64\\Release\\openvino_genai.lib",
            ],
            "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
        }
    ]
}
