import { Vector2 } from './types.js';
let mousePos = new Vector2(0, 0);

export function OnMouseMove(event)
{
  mousePos = event.data.global;
}

export function GetMousePosition()
{
  return mousePos;
}