const fs = require('fs');
const helper = require('./helper');

function mergePinyin(words) {
  let pinyin = '';
  for (let i = 3; i < words.length; i++) {
    pinyin += words[i];
  }

  return pinyin;
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
    const pinyin = mergePinyin(words);
    const key = helper.mapStr(pinyin);
    const notExist = !obj[key];

    if (!key) {
      return;
    }

    if ((lang === 'zh_cn') && tw) {
      /*skip tranditional words if lang === 'zh_cn'*/
      return;
    }
    
    if ((lang !== 'zh_cn') && !tw) {
      /*skip simpile Chinese words if lang === 'zh_cn'*/
      return;
    }

    console.log(`${pinyin} => ${key}`);

    if (notExist) {
      obj[key] = {
        pinyin: '',
        key: key,
        items: []
      }
    }

    let value = obj[key];
    if (value.pinyin.indexOf(' ' + pinyin) < 0) {
      value.pinyin = value.pinyin + ' ' + pinyin;
    }

    value.items.push({
      pinyin: pinyin,
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

    result += `/*${value.pinyin}*/\n`;
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
    let pinyin = iter.items[0].pinyin;
    result += `  {"${key}", "${pinyin}", s_${key}},\n`
  });

  result += '};\n';

  helper.saveResult(`t9_${lang}.inc`, result);
}

toT9('chinese_words.txt', 'zh_cn');
