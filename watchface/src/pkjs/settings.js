/* Clay supplies a local settings page; the watch persists the chosen appearance. */
var config = require('./config.json');

function choice(value, maximum) {
  if (typeof value !== 'string' && typeof value !== 'number') return 0;
  var number = Number(value);
  return isFinite(number) && Math.floor(number) === number &&
    number >= 0 && number <= maximum ? number : 0;
}

function settingsMessage(raw) {
  var saved;
  try { saved = JSON.parse(raw) || {}; } catch (error) { saved = {}; }
  return {
    SETTING_PALETTE: choice(saved.SETTING_PALETTE, 2),
    SETTING_FONT: choice(saved.SETTING_FONT, 1),
    SETTING_LOCATION: choice(saved.SETTING_LOCATION, 7),
    SETTING_DATE: choice(saved.SETTING_DATE, 3)
  };
}

function initialize(onReady) {
  var Clay = require('@rebble/clay');
  var clay = new Clay(config, null, { autoHandleEvents: false });
  function send(raw, done) {
    Pebble.sendAppMessage(settingsMessage(raw), function () {
      if (done) done();
    }, function (error) {
      console.log('settings send failed: ' + JSON.stringify(error));
      if (done) done();
    });
  }
  Pebble.addEventListener('showConfiguration', function () {
    Pebble.openURL(clay.generateUrl());
  });
  Pebble.addEventListener('webviewclosed', function (event) {
    if (!event || !event.response || event.response === 'CANCELLED') return;
    try { clay.getSettings(event.response); }
    catch (error) { console.log('settings rejected: ' + error); return; }
    send(localStorage.getItem('clay-settings'));
  });
  var saved = localStorage.getItem('clay-settings');
  if (saved) send(saved, onReady);
  else if (onReady) onReady();
}

module.exports = { initialize: initialize, settingsMessage: settingsMessage };
