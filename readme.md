# Node.js OpenVINO LLM C++ addon

This is a Node.js addon for [OpenVINO GenAI](https://github.com/openvinotoolkit/openvino.genai/) LLM.
Tested using TinyLLama chat 1.1 OpenVINO int4 model on Windows 11.

Watch below YouTube video for demo :

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/dAk8rlFE3QE/0.jpg)](https://www.youtube.com/watch?v=dAk8rlFE3QE)

## Build

- Visual Studio 2022 (C++)
- Node 20.11 or higher
- node-gyp
- Python 3.11
- Used [openvino_genai_windows_2024.2.0.0_x86_64](https://docs.openvino.ai/2024/get-started/install-openvino.html?PACKAGE=OPENVINO_BASE&VERSION=v_2024_2_0&OP_SYSTEM=WINDOWS&DISTRIBUTION=ARCHIVE) release

Run the following commands to build:

```
npm install
node-gyp configure
node-gyp build
```

## Run

To test the Node.js OpenVINO LLM addon run the `index.js` script.

`node index.js D:/demo/TinyLlama-1.1B-Chat-v1.0-openvino-int4`

Disable streaming

`node index.js D:/demo/TinyLlama-1.1B-Chat-v1.0-openvino-int4 nostream`

## Supported models

Supported models are [here](https://github.com/openvinotoolkit/openvino.genai/blob/releases/2024/2/src/docs/SUPPORTED_MODELS.md)
