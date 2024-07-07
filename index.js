const ovllm = require('./build/Release/ovllm');
const readline = require('readline');
//-openvino-int4
ovllm.initialize("F:\\om\\2024\\LLM\\converter\\TinyLlama-1.1B-Chat-v1.0-openvino-int4", "CPU");
//ovllm.initialize("F:\\om\\2024\\LLM\\converter\\phi-2-openvino-int4", "CPU");
let token_count = 0;
function onStream(word) {
    token_count += 1;
    process.stdout.write(word);
}

const reader = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

function chatInterface() {
    reader.question('You: ', (message) => {
        if (message.toLowerCase() === 'exit') {
            reader.close();
            return;
        }
        console.log("AI:")
        const startTime = new Date();
        ovllm.generateStream(message, onStream)
        const elapsedTime = (new Date() - startTime) / 1000;
        console.log("\n");
        console.log(Math.floor(token_count / elapsedTime), "Tokens/sec\n");
        chatInterface();
    });
}

console.log('OpenVINO LLM fast chat interface! Type "exit" to quit.');


chatInterface();


reader.on('close', () => {
    console.log('Bye...');
    ovllm.cleanup()
    process.exit(0);
});

