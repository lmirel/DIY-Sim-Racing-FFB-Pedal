function listGamepads() {
  const gamepads = navigator.getGamepads();
  const gamepadList = document.getElementById("gamepad-list");
  gamepadList.innerHTML = ""; // Clear the list

  for (let i = 0; i < gamepads.length; i++) {
    const gamepad = gamepads[i];
    if (gamepad) { // Check if gamepad is connected
      const gamepadItem = document.createElement("li");
      gamepadItem.textContent = `Gamepad ${i}: ${gamepad.id}`;
      gamepadList.appendChild(gamepadItem);
    }
  }
}

// Call listGamepads initially to populate the list
listGamepads();

// Add an event listener for when gamepads are connected or disconnected
window.addEventListener("gamepadconnected", listGamepads);
window.addEventListener("gamepaddisconnected", listGamepads);