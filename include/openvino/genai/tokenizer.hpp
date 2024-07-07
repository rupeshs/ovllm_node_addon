// Copyright (C) 2023-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <initializer_list>
#include <openvino/runtime/tensor.hpp>
#include "openvino/genai/visibility.hpp"

namespace ov {
namespace genai {

using ChatHistory = std::vector<std::unordered_map<std::string, std::string>>;

struct TokenizedInputs {
    ov::Tensor input_ids;
    ov::Tensor attention_mask;
};

/**
* @brief class is used to encode prompts and decode resulting tokens
*/
class OPENVINO_GENAI_EXPORTS Tokenizer {
public:
    /**
    * @brief ov::Tokenizer constructor.
    * @param tokenizer_path openvino_tokenizer.xml and openvino_detokenizer.xml should be located in the tokenizer_path
    */
    Tokenizer(const std::string& tokenizer_path);

    /**
    * @brief encode a single prompt
    * @return pair of [input_ids, attention_mask]
    */
    TokenizedInputs encode(const std::string prompt);
    
    /**
    * @brief encode batch of prompts. Left padding will be applied by default
    * @param prompts vector storing batch of prompts
    * @return pair of [input_ids, attention_mask]
    */
    TokenizedInputs encode(std::vector<std::string>& prompts);
    TokenizedInputs encode(std::vector<std::string>&& prompts);
    TokenizedInputs encode(std::initializer_list<std::string>& prompts);
    
    /**
    * @brief decode sequence of tokens
    * @param tokens vector storing tokens
    * @return sequence string
    */
    std::string decode(std::vector<int64_t> tokens);
    
    /**
    * @brief decode tokens. 
    * @param tokens ov::Tensor with tokens with shape [batch_size, seq_len]
    * @return vector of std::string, with size = batch_size
    */
    std::vector<std::string> decode(ov::Tensor tokens);

    /**
    * @brief batched decoding of tokens. 
    * @param tokens vector of vectors with tokens, tokens.size() is equal to batch_size
    * @return vector of std::string, with size equal to batch_size
    */
    std::vector<std::string> decode(std::vector<std::vector<int64_t>> tokens);

    /**
     * @brief Embeds input prompts with special tags for a chat scenario.
     * 
     * For example, for Qwen family models, the prompt "1+1=" would be transformed into 
     * <|im_start|>user\n1+1=<|im_end|>\n<|im_start|>assistant\n.
     *
     * @param history A vector of maps, with chat history, e.g. [{"role": "user", "content": "prompt"}, ...].
     * @param add_generation_prompt Whether to add an ending that indicates the start of generation.
     * @param chat_template An optional chat template string, if not specified will be taken from the tokenizer.
     * @return A string with the transformed and concatenated prompts from the chat history.
     * @throws Exception if the chat template was unable to parse the input history.
     */
    std::string apply_chat_template(const ChatHistory& history, 
                                    bool add_generation_prompt, 
                                    const std::string& chat_template="") const;

    // information about <bos>, <eos> tokens should be public,
    // they are used at least in StreamerBase descendants
    int64_t get_bos_token_id() const;
    int64_t get_eos_token_id() const;
    int64_t get_pad_token_id() const;

    std::string get_bos_token() const;
    std::string get_eos_token() const;
    std::string get_pad_token() const;

    Tokenizer() = default;
    ~Tokenizer();
private:
    class TokenizerImpl;
    std::shared_ptr<TokenizerImpl> m_pimpl;
};
}  // namespace genai
}  // namespace ov
