var ws = new WebSocket(`ws://${window.location.hostname}/ws`);
ws.onmessage = (event) => console.log(event.data);

function sendCommand(cmd) {
ws.send(cmd);
}

const logDiv = document.getElementById('log');
  const ws = new WebSocket(`ws://${window.location.hostname}/ws`);

  ws.onmessage = (event) => {
    logDiv.innerHTML += event.data + "<br>";
    logDiv.scrollTop = logDiv.scrollHeight; // Auto-scroll
  }