const fs = require('fs');

// console.log(process.argv);

const main = () => {
    try {
        if (process.argv.length < 5) {
            throw new Error('not enough arguments');
        }

        const op = process.argv[2];

        const input = process.argv[3];
        const output = process.argv[4];

        if (op == 'encode') {
            encode(input, output);
        } else if (op == 'decode') {
            decode(input, output);
        } else {
            throw new Error(`Invalid operation ${op}`);
        }
    } catch (error) {
        console.log(`execution failed ${error}`);
        console.log('usage endcode/decode {input_file_name} {output_file_name}');
    }
}

const encode = (input, output) => {
    console.log('encode', input, output);
    throw new Error('not implemented');
}

const decode = (input, output) => {
    console.log('decode', input, output);
    throw new Error('not implemented');
}

main();