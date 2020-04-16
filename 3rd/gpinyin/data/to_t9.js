const fs = require('fs');

const mapTable = {
  'a': '1',
  'b': '1',
  'c': '1',
  'd': '2',
  'e': '2',
  'f': '2',
  'g': '3',
  'h': '3',
  'i': '3',
  'j': '4',
  'k': '4',
  'l': '4',
  'm': '5',
  'n': '5',
  'o': '5',
  'p': '6',
  'q': '6',
  'r': '6',
  's': '6',
  't': '7',
  'u': '7',
  'v': '7',
  'w': '8',
  'x': '8',
  'y': '8',
  'z': '8'
};

function mapChar(c) {
  return mapTable[c]
}

function mapStr(str) {
  s = ''

  if (!str) {
    return s;
  }

  for (let i = 0; i < str.length; i++) {
    s += mapChar(str[i])
  }

  return s;
}

function saveResult(filename, result) {
  const BOM = "\ufeff";

  fs.writeFileSync(filename, BOM + result);
  console.log(`write result to ${filename}`);
}

function getPinyin(words) {
  let py = '';
  for(let i = 3; i < words.length; i++) {
    py += words[i];
  }

  return py;
}

function toT9(filename, lang) {
  let obj = {}
  let result = ''
  const content = fs.readFileSync(filename, "utf16le");
  const lines = content.split('\n');

  lines.forEach((line, index) => {
    const words = line.split(' ');
    const char = words[0];
    const freq = parseFloat(words[1]);
    const tw = words[2] === '1';
    const py = getPinyin(words);
    const key = mapStr(py);
    const notExist = !obj[key];

    if (!key) {
      return;
    }

    if (tw) {
      return;
    }

    console.log(`${py} => ${key}`);
    if (notExist) {
      obj[key] = {
        py: '',
        key: key,
        items: []
      }
    }

    let value = obj[key];
    if (value.py.indexOf(' ' + py) < 0) {
      value.py = value.py + ' ' + py;
    }

    value.items.push({
      char: char,
      freq: freq
    });

  });

  let arr = []
  for (let key in obj) {
    let value = obj[key];
    if (value.items.length == 0) {
      continue;
    }

    value.items.sort((a, b) => {
      return b.freq - a.freq;
    });

    result += `/*${value.py}*/\n`;
    result += `static const char* s_${key}[] = {\n`
    value.items.forEach(iter => {
      result += `  \"${iter.char}\",\n`;
    });
    result += '  NULL\n';
    result += '};\n';
    
    arr.push(value);
  }

  arr.sort((a, b) => {
    return a.key.localeCompare(b.key);
  });

  result += `static const t9_item_info_t s_${lang}_items[] = {\n`;
  arr.forEach(iter => {
    let key = iter.key;
    let py = iter.py.split(' ')[1];

    result += `  {"${key}", "${py}", s_${key}},\n`
  });

  result += '};\n';

  saveResult("t9_zh_cn.inc", result);
}

toT9('rawdict_utf16_65105_freq.txt', 'zh_cn');

