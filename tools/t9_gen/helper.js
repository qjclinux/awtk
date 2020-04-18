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

function mergePinyin(words) {
  let pinyin = '';
  for (let i = 3; i < words.length; i++) {
    pinyin += words[i];
  }

  return pinyin;
}


module.exports = {
 saveResult:saveResult,
  mergePinyin : mergePinyin,
  mapStr : mapStr
};
