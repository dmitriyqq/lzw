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

class Library {
    constructor(code, letters) {
        this.code = code;
        this.letters = letters
    }
}

const encode = (input, output) => {
    console.log('encode', input, output);

    let orig_data = fs.readFileSync(input);
    let str = ""
    for (let i = 0; i < orig_data.length; i++) {
        str += String.fromCharCode(orig_data[i]);
    }
    orig_data = str;
    console.log("number chars in input", orig_data.length)
    console.log('in size', orig_data.length, 'byte');
    // console.log('orig_data', orig_data)
    // const t = []
    // for (let i = 0; i < str.length; i++){
    //     t.push(str.charCodeAt(i))
    // }

    var dict = new Map();

    const MAX_ASCII = 255;
    for (let i = 0; i <= MAX_ASCII; i++) {
        const char = String.fromCharCode(i);
        dict.set(char, dict.size);
    }

    let out_data = [];

    for (let i = 0; i < orig_data.length;) {

        const char = orig_data[i]
        let current_seq = char;
        let ii = i + 1;

        // console.log('current_seq ->', current_seq, 'i:', i, Array.from(dict));

        // console.log(dict.has(Buffer.from([1])))

        while (dict.has(current_seq) && ii < orig_data.length) {
            // console.log('dict has', current_seq);
            current_seq += orig_data[ii];// Buffer.concat([current_seq, Buffer.from([orig_data[ii]])]);
            ii++;
        }

        i = ii - 1;
        // console.log('ii = ', ii)
        // console.log('Exit from loop ->', current_seq);

        if (!dict.has(current_seq)) {

            // copy string to dict
            const new_symbol = current_seq;
            // console.log('##### add to dict', new_symbol, dict.size)
            dict.set(new_symbol, dict.size)

            // Strip last byte from buffer
            // const new_buffer = Buffer.alloc(current_seq.length-1);
            // current_seq.copy(new_buffer, 0, 0, current_seq.length-1)
            current_seq = current_seq.substring(0, current_seq.length - 1);
            // current_seq = new_buffer;

            // console.log('step back ->', current_seq, new_symbol);
            if (!dict.has(current_seq)) {
                // console.log(current_seq.length);
                throw new Error(`key ${current_seq} is not found in seq: ${Array.from(dict)}`);
            }
            // console.log('=== write', current_seq, dict.get(current_seq))
            out_data.push(dict.get(current_seq));
        } else {
            // console.log('=== write2', current_seq, dict.get(current_seq))
            out_data.push(dict.get(current_seq));
            break;
        }
    }

    console.log('Number chars in encoded', out_data.length)

    const bit_counter = new Map();
    // bit_counter.set(1, 0)
    // bit_counter.set(2, 0)
    // bit_counter.set(3, 0)
    // bit_counter.set(4, 0)
    
    // for (let i = 0; i < out_data.length; i++) {
        
        
    //     if (ch <= 255) {
    //         bit_counter.set(1, bit_counter.get(1) + 1);
    //     } 
    //     else if (ch <= 255 * 255) {
    //         bit_counter.set(2, bit_counter.get(2) + 1);
    //     }
    //     else if (ch <= 255 * 255 * 255) {
    //         bit_counter.set(3, bit_counter.get(3) + 1);
    //     }
    //     else if (ch <= 255 * 255 * 255 * 255) {
    //         bit_counter.set(4, bit_counter.get(4) + 2);
    //     }
    // }
    
    let j = 0;
    let output_size = 0;
    while (j < out_data.length) {
        let max = 0;
        let counter = 0;
        for (let i = 0; i < 1024; i++) {
            if (j >= out_data.length) {
                break;
            }
            const ch = out_data[j];
            counter++;
            max = Math.max(ch, max);
            j++;
        }
        const ws = max.toString(2).length;
        output_size += ws * counter;
        // console.log('max', max.toString(2).length);
        if (bit_counter.has(ws)) {
            bit_counter.set(ws, bit_counter.get(ws)+ 1);
        } else {
            bit_counter.set(ws, 1);
        }
    }

    const wordSize = ((dict.size-1).toString(2).length);

    console.log('out size', (out_data.length * wordSize) / 8, 'byte');
    console.log('out size(optimized)', (output_size) / 8, 'byte');
    console.log(Array.from(bit_counter));
    fs.writeFileSync(output, new Uint32Array(out_data));
}

const decode = (input, output) => {


    console.log(input);
    var libs = [];
    const MAX_ASCII = 255;

    for (let i = 0; i <= MAX_ASCII; i++) {
        const char = String.fromCharCode(i);
        libs.push(new Library(i, char));
    }

    let coder = MAX_ASCII + 1;
    let current = "";
    let out = "";

    const data = JSON.parse(fs.readFileSync(input, 'utf-8'));
    // console.log(data);
    // console.log(libs);

    for (let i = 0; i < data.length; i++) {
        // console.log(i, data[i], libs[data[i]]);
        current += libs[data[i]].letters;
        //console.log("code: " + libs[encode[i]].code + "---current: "+current);
        out += current;
        //console.log("------output: "+output);

        let newLib = new Library(coder, current);
        libs.push(new Library(newLib.code, newLib.letters));

        if (i < data.length - 1) {
            libs[coder].letters += libs[data[i + 1]].letters.charAt(0);
        }
        ++coder;
        current = "";
    }
    // console.log("out ", out);
    fs.writeFileSync(output, out, 'ascii');
}

main();