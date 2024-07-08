const ovllm = require('./build/Release/ovllm');
const readline = require('readline');
const llmPath = process.argv[2];
let streaming = true;

if (process.argv.length === 2) {
    console.error('Error: OpenVINO model path is required.');
    process.exit(1);
}
if (process.argv[3] === "nostream") {
    streaming = false;
}

ovllm.initialize(llmPath, "CPU", streaming);

let token_count = 0;

if (process.argv.length === 2) {
    console.error('Error: OpenVINO model path is required.');
    process.exit(1);
}
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

        if (streaming) {
            const startTime = new Date();
            ovllm.generateStream(message, onStream)
            const elapsedTime = (new Date() - startTime) / 1000;
            console.log("\n");
            console.log(Math.floor(token_count / elapsedTime), "Tokens/sec\n");
        }
        else {
            let response = ovllm.generate(message);
            console.log(response);
        }

        chatInterface();
    });
}

console.log('OpenVINO LLM Node.js fast chat interface! Type "exit" to quit.');
chatInterface();
reader.on('close', () => {
    console.log('Bye...');
    ovllm.cleanup()
    process.exit(0);
});

ovllm.initialize(llmPath, "CPU");

console.log('OpenVINO LLM Node.js fast chat interface! Type "exit" to quit.\n');
chatInterface();
