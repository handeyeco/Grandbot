/**
 * Helper for generating table of contents in Markdown files
 */

const fs = require("fs");

const endString = `[//]: # (__endtoc__)`;

// removes any existing TOC if it exists
function removeTOC(input) {
  let next = input;
  const index = next.indexOf(endString);
  if (index >= 0) {
    next = input.slice(index + endString.length);
  }
  return next.trim();
}

// converts arbitrary strings into Markdown links
function stringToLink(str) {
  const anchor = str
    .trim()
    .toLowerCase()
    // only keep alphanumeric, spaces, and dashes
    .replaceAll(/[^a-z0-9 -]/g, "")
    // replace spaces with dashes
    .replaceAll(/\s+/g, "-");
  const text = str.trim();
  return `[${text}](#${anchor})`;
}

// generate a new TOC
function generateTOC(input) {
  let toc = "";
  input.split("\n").forEach((line) => {
    // ignore H1, only handle H2+
    const match = line.match(/^(#{2,})(.+)/);
    if (match) {
      const a = stringToLink(match[2]);
      const indent = "  ".repeat(match[1].length - 2);
      toc += `${indent}- ${a}\n`;
    }
  });
  return `${toc}\n${endString}\n\n`;
}

function main() {
  const file = process.argv[2];
  if (!file) {
    throw new Error("file required: node toc.js [PATH]");
  }
  const data = fs.readFileSync(file, "utf-8");
  const doc = removeTOC(data);
  const toc = generateTOC(doc);
  const combined = `${toc}${doc}`;
  fs.writeFileSync(file, combined);
}

main();
