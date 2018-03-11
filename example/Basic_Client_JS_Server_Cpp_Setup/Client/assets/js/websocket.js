(function(window) {
  var ws, wsp;
  var msg_number = 0;
  var msg;
  window.onload = function() {
    ws = new WebSocket("ws://localhost:8000/broadcast");
    ws.onmessage = function(evt) {
      document.getElementById('message_r').innerHTML = evt.data;
      ws.send("Recieved");
      document.getElementById('message_s').innerHTML = "Recieved";
    };
    ws.onopen = function() {
      ws.send("Hand Shake");
      console.log("[Broadcast]connection opened for communication");
      document.getElementById('status').innerHTML = "Connected";
    };

    wsp = new WebSocket("ws://localhost:8000/private");
    wsp.onmessage = function(evt) {
      document.getElementById('message_rp').innerHTML = evt.data;
      setTimeout(function() {
        msg_number++;
        msg = "Do Task :" + msg_number.toString();
        wsp.send(msg);
        document.getElementById('message_sp').innerHTML = msg;
      }, 1000);
    };
    wsp.onopen = function() {
      wsp.send("Hand Shake");
      console.log("[Private]connection opened for communication");
      document.getElementById('statusp').innerHTML = "Connected";
    };
  };
  window.onclose = function() {
    ws.close();
    wsp.close();
  };
})(window);
