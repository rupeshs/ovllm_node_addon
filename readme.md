# Node.js OpenVINO LLM C++ addon

This is a Node.js addon for [OpenVINO GenAI](https://github.com/openvinotoolkit/openvino.genai/) LLM.
Tested using TinyLLama chat 1.1 OpenVINO int4 model on Windows 11.

## Build

- Visual Studio 2022 (C++)
- Node 20.11 or higher
- node-gyp
- Python 3.11

Run the following commands to build:

```
npm install
node-gyp configue
node-gyp build
```

## Run

To test the Node.js OpenVINO LLM addon run the `index.js` script.

`node index.js D:/demo/TinyLlama-1.1B-Chat-v1.0-openvino-int4`

Disable streaming

`node index.js D:/demo/TinyLlama-1.1B-Chat-v1.0-openvino-int4 nostream`

## Supported models

Supported models are [here](https://github.com/openvinotoolkit/openvino.genai/blob/releases/2024/2/src/docs/SUPPORTED_MODELS.md)
