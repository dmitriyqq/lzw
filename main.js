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

    orig_data = fs.readFileSync(input, 'utf8');

    var dict = new Map();
    var set = new Set()

    for (const char of orig_data) {
        if (!set.has(char)) {
            set.add(char);
        }
    }

    for (const char of Array.from(set).sort()) {
        dict.set(char, dict.size);
    }

    let out_data = [];

    for (const [key, _] of dict) {
        // console.log(key)
        out_data.push(key);
    }

    for (let i = 0; i < orig_data.length;) {

        const char = orig_data[i]
        let current_seq = char;
        let ii = i+1;

        // console.log('current_seq ->', current_seq, 'i:', i, Array.from(dict));

        while(dict.has(current_seq) && ii < orig_data.length)
        {
            // console.log('dict has', current_seq);
            current_seq += orig_data[ii];
            ii++;
        }

        i = ii-1;
        // console.log('ii = ', ii)
        // console.log('Exit from loop ->', current_seq);

        if (!dict.has(current_seq)) {
            
            // copy string to dict
            const new_symbol = current_seq;
            // console.log('##### add to dict', new_symbol, dict.size)
            dict.set(new_symbol, dict.size)

            current_seq = current_seq.substring(0, current_seq.length-1);

            // console.log('step back ->', current_seq, new_symbol);
            if (!dict.has(current_seq)) {
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

    console.log('out data ', out_data)
    fs.writeFileSync(output, JSON.stringify(out_data));
}

const decode = (input, output) => {
    class Library {
        constructor(code, letters) {
            this.code = code;
            this.letters = letters
        }
    }

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

    for (let i = 0; i < input.length; i++) {
        current += libs[input[i]].letters;
        //console.log("code: " + libs[encode[i]].code + "---current: "+current);
        out += current;
        //console.log("------output: "+output);

        let newLib = new Library(coder, current);
        libs.push(new Library(newLib.code, newLib.letters));

        if (i < encode.length - 1) {
            libs[coder].letters += libs[encode[i + 1]].letters.charAt(0);
        }
        ++coder;
        current = "";
    }
    console.log("out ", out);
    fs.writeFileSync(output, JSON.stringify(out));
}

main();