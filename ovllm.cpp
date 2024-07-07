#include <napi.h>
#include "openvino/genai/llm_pipeline.hpp"

static ov::genai::LLMPipeline *pipe = nullptr;

Napi::Value Initialize(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString())
    {
        Napi::TypeError::New(env, "Expected LLM path and device arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    std::string llmPath = info[0].As<Napi::String>().Utf8Value();
    std::string device = info[1].As<Napi::String>().Utf8Value();

    std::cout << "OpenVINO LLM: " << llmPath << std::endl;
    std::cout << "Device : " << device << std::endl;

    pipe = new ov::genai::LLMPipeline(llmPath, device);
    pipe->start_chat();
    return Napi::Boolean::New(env, true);
}

Napi::Value Generate(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (pipe == nullptr)
    {
        Napi::TypeError::New(env, "Pipe is not initialized").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (info.Length() < 1 || !info[0].IsString())
    {
        Napi::TypeError::New(env, "Expected a prompt").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    std::string prompt = info[0].As<Napi::String>().Utf8Value();

    std::cout << pipe->generate(prompt);
    return Napi::Boolean::New(env, true);
}
Napi::Value GenerateStream(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (pipe == nullptr)
    {
        Napi::TypeError::New(env, "Pipe not initialized").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Expected two arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(env, "Expected a prompt").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!info[1].IsFunction())
    {
        Napi::TypeError::New(env, "Expected callback function as the second argument").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    // Js callback
    Napi::Function callback = info[1].As<Napi::Function>();

    std::string prompt = info[0].As<Napi::String>().Utf8Value();
    ov::genai::GenerationConfig config;
    config.max_new_tokens = 256;

    std::function<bool(std::string)> streamer = [callback, env](std::string word)
    {
        callback.Call(env.Global(), {Napi::String::New(env, word)});
        return false;
    };

    pipe->generate(prompt, config, streamer);
    return Napi::Boolean::New(env, true);
}
Napi::Value Cleanup(const Napi::CallbackInfo &info)
{
    pipe->finish_chat();
    Napi::Env env = info.Env();
    if (pipe != nullptr)
    {
        delete pipe;
        pipe = nullptr;
    }
    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "initialize"), Napi::Function::New(env, Initialize));
    exports.Set(Napi::String::New(env, "generate"), Napi::Function::New(env, Generate));
    exports.Set(Napi::String::New(env, "generateStream"), Napi::Function::New(env, GenerateStream));
    exports.Set(Napi::String::New(env, "cleanup"), Napi::Function::New(env, Cleanup));
    return exports;
}

NODE_API_MODULE(ovllmnodeaddon, Init)
