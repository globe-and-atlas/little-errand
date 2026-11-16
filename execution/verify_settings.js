const assert = require('assert');
const fs = require('fs');
const path = require('path');
const vm = require('vm');

const bundle = fs.readFileSync(path.join(__dirname, '..', 'watchface', 'build',
  'pebble-js-app.js'), 'utf8');
const storage = {};

function boot() {
  const events = {};
  const sent = [];
  let opened = '';
  const context = {
    Pebble: {
      addEventListener: (name, callback) => { events[name] = callback; },
      openURL: (url) => { opened = url; },
      sendAppMessage: (message, success) => { sent.push(message); if (success) success(); }
    },
    localStorage: {
      getItem: (key) => storage[key] || null,
      setItem: (key, value) => { storage[key] = value; }
    },
    navigator: { geolocation: { getCurrentPosition: (_ok, fail) => fail({ message: 'offline' }) } },
    XMLHttpRequest: function () {},
    setInterval: function () {},
    console: { log: function () {} }
  };
  vm.runInNewContext(bundle, context);
  events.ready();
  return { events, sent, get opened() { return opened; } };
}

const first = boot();
first.events.showConfiguration();
assert(first.opened.startsWith('data:text/html'));
assert(decodeURIComponent(first.opened).includes('Large light'));
assert(decodeURIComponent(first.opened).includes('Travel through locations'));
assert(decodeURIComponent(first.opened).includes('YYYY-MM-DD'));
first.events.webviewclosed({ response: encodeURIComponent(JSON.stringify({
  SETTING_PALETTE: { value: '2' }, SETTING_FONT: { value: '1' },
  SETTING_LOCATION: { value: '7' }, SETTING_DATE: { value: '3' }
})) });
assert.strictEqual(first.sent[first.sent.length - 1].SETTING_PALETTE, 2);
assert.strictEqual(first.sent[first.sent.length - 1].SETTING_FONT, 1);
assert.strictEqual(first.sent[first.sent.length - 1].SETTING_LOCATION, 7);
assert.strictEqual(first.sent[first.sent.length - 1].SETTING_DATE, 3);
const before = first.sent.length;
first.events.webviewclosed({ response: 'CANCELLED' });
assert.strictEqual(first.sent.length, before);
const second = boot();
assert.strictEqual(second.sent[0].SETTING_PALETTE, 2);
assert.strictEqual(second.sent[0].SETTING_FONT, 1);
assert.strictEqual(second.sent[0].SETTING_LOCATION, 7);
assert.strictEqual(second.sent[0].SETTING_DATE, 3);
console.log('PASS packaged settings page, save, cancel, and phone restore');
