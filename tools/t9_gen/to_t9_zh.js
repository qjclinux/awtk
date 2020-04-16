const fs = require('fs');
const helper = require('./helper');

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
    const key = helper.mapStr(py);
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
      py : py,
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
    let py = iter.items[0].py;
    result += `  {"${key}", "${py}", s_${key}},\n`
  });

  result += '};\n';

  helper.saveResult(`t9_${lang}.inc`, result);
}

toT9('chinese_words.txt', 'zh_cn');

