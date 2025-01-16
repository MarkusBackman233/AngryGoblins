
import { GetMousePosition } from './input.js';
import { Vector2 } from './types.js';
let currentDragCard = null;
let currentDragOffset = new Vector2(0, 0);

export function RegisterDragItem(item)
{
  item.eventMode = 'static';
  item.on('pointerdown', (event) => {
      currentDragCard = event.target;
      currentDragOffset.x = currentDragCard.x - GetMousePosition().x;
      currentDragOffset.y = currentDragCard.y - GetMousePosition().y;
  });

  item.on('pointerup', (event) => {
    currentDragCard = null;
  }); 
}

export function HandleDragItem(deltaTime)
{
  if (currentDragCard != null)
  {
    let xVelocity = ( GetMousePosition().x + currentDragOffset.x - currentDragCard.x) * deltaTime;

    currentDragCard.angle = xVelocity;
    currentDragCard.x += xVelocity;
    currentDragCard.y += ( GetMousePosition().y + currentDragOffset.y - currentDragCard.y) * deltaTime;
  }
}

export function GetCurrentDragItem()
{
  return currentDragCard;
}

export function GetCurrentDragOffset()
{
  return currentDragOffset;
}