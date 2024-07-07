// Copyright (C) 2023-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <limits>
#include <variant>
#include <string>

#include "openvino/runtime/compiled_model.hpp"
#include "openvino/runtime/infer_request.hpp"
#include "openvino/genai/tokenizer.hpp"

namespace ov {
namespace genai {

/**
 * @brief controls the stopping condition for grouped beam search. The following values are possible:
 *        "EARLY" stops as soon as there are `num_beams` complete candidates.
          "HEURISTIC" stops when is it unlikely to find better candidates.
          "NEVER" stops when there cannot be better candidates.
 */
enum class StopCriteria { EARLY, HEURISTIC, NEVER };

/**
 * @brief Structure to keep generation config parameters. For a selected method of decoding, only parameters from that group
 * and generic parameters are used. For example, if do_sample is set to true, then only generic parameters and random sampling parameters will 
 * be used while greedy and beam search parameters will not affect decoding at all.
 * 
 * Generic parameters:
 * @param max_length the maximum length the generated tokens can have. Corresponds to the length of the input prompt +
 *        `max_new_tokens`. Its effect is overridden by `max_new_tokens`, if also set.
 * @param max_new_tokens the maximum numbers of tokens to generate, excluding the number of tokens in the prompt. max_new_tokens has priority over max_length.
 * @param ignore_eos if set to true, then generation will not stop even if <eos> token is met.
 * @param eos_token_id token_id of <eos> (end of sentence)
 * 
 * Beam search specific parameters:
 * @param num_beams number of beams for beam search. 1 disables beam search.
 * @param num_beam_groups number of groups to divide `num_beams` into in order to ensure diversity among different groups of beams.
 * @param diversity_penalty this value is subtracted from a beam's score if it generates the same token as any beam from other group at a
 *        particular time. See https://arxiv.org/pdf/1909.05858.
 * @param length_penalty exponential penalty to the length that is used with beam-based generation. It is applied as an exponent to
 *        the sequence length, which in turn is used to divide the score of the sequence. Since the score is the log
 *        likelihood of the sequence (i.e. negative), `length_penalty` > 0.0 promotes longer sequences, while
 *        `length_penalty` < 0.0 encourages shorter sequences.
 * @param num_return_sequences the number of sequences to return for grouped beam search decoding per batch element. num_return_sequences must be less or equel to num_beams.
 * @param no_repeat_ngram_size if set to int > 0, all ngrams of that size can only occur once.
 * @param stop_criteria controls the stopping condition for grouped beam search. It accepts the following values: 
 *        "EARLY", where the generation stops as soon as there are `num_beams` complete candidates; "HEURISTIC", where an 
 *        "HEURISTIC" is applied and the generation stops when is it very unlikely to find better candidates;
 *        "NEVER", where the beam search procedure only stops when there cannot be better candidates (canonical beam search algorithm).
 * 
 * Random sampling parameters:
 * @param temperature the value used to modulate token probabilities for random sampling.
 * @param top_p - if set to float < 1, only the smallest set of most probable tokens with probabilities that add up to top_p or higher are kept for generation.
 * @param top_k the number of highest probability vocabulary tokens to keep for top-k-filtering.
 * @param do_sample whether or not to use multinomial random sampling that add up to `top_p` or higher are kept.
 * @param repetition_penalty the parameter for repetition penalty. 1.0 means no penalty.
 */
class OPENVINO_GENAI_EXPORTS GenerationConfig {
public:
    GenerationConfig() = default;
    explicit GenerationConfig(const std::string& json_path);

    // Generic
    size_t max_new_tokens = SIZE_MAX;
    size_t max_length = SIZE_MAX;
    bool ignore_eos = false;

    // Beam search specific
    size_t num_beam_groups = 1;
    size_t num_beams = 1;
    float diversity_penalty = 1.0f;
    float length_penalty = 1.0f;
    size_t num_return_sequences = 1;
    size_t no_repeat_ngram_size = std::numeric_limits<size_t>::max();
    StopCriteria stop_criteria = StopCriteria::HEURISTIC;
    
    // Multinomial
    float temperature = 1.0f;
    float top_p = 1.0f;
    size_t top_k = 50;
    bool do_sample = false;
    float repetition_penalty = 1.0f;

    // EOS special token
    int64_t eos_token_id = -1;

    size_t get_max_new_tokens(size_t prompt_length = 0) const;
    bool is_greedy_decoding() const;
    bool is_beam_search() const;
    bool is_multinomial() const;
    void update_generation_config(const ov::AnyMap& config_map = {});
    
    template <typename... Properties>
    util::EnableIfAllStringAny<void, Properties...> update_generation_config(Properties&&... properties) {
        return update_generation_config(AnyMap{std::forward<Properties>(properties)...});
    }
    
    /// @brief checks that are no conflicting parameters, e.g. do_sample=true and num_beams > 1.
    /// @throws Exception if config is invalid.
    void validate() const;
};

/*
 * utils that allow to use generate and operator() in the following way:
 * pipe.generate(input_ids, ov::genai::max_new_tokens(200), ov::genai::temperature(1.0f),...)
 * pipe(text, ov::genai::max_new_tokens(200), ov::genai::temperature(1.0f),...)
*/
static constexpr ov::Property<size_t> max_new_tokens{"max_new_tokens"};
static constexpr ov::Property<size_t> max_length{"max_length"};
static constexpr ov::Property<bool> ignore_eos{"ignore_eos"};

static constexpr ov::Property<size_t> num_beam_groups{"num_beam_groups"};
static constexpr ov::Property<size_t> num_beams{"num_beams"};
static constexpr ov::Property<float> diversity_penalty{"diversity_penalty"};
static constexpr ov::Property<float> length_penalty{"length_penalty"};
static constexpr ov::Property<size_t> num_return_sequences{"num_return_sequences"};
static constexpr ov::Property<size_t> no_repeat_ngram_size{"no_repeat_ngram_size"};
static constexpr ov::Property<StopCriteria> stop_criteria{"stop_criteria"};

static constexpr ov::Property<float> temperature{"temperature"};
static constexpr ov::Property<float> top_p{"top_p"};
static constexpr ov::Property<int> top_k{"top_k"};
static constexpr ov::Property<bool> do_sample{"do_sample"};
static constexpr ov::Property<float> repetition_penalty{"repetition_penalty"};
static constexpr ov::Property<int64_t> eos_token_id{"eos_token_id"};

}  // namespace genai
}  // namespace ov
