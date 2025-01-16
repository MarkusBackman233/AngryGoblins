import { Application, Assets, Sprite,Text, TextStyle, Container, Color } from 'https://cdn.skypack.dev/pixi.js';
import { GetJsonCard, GetCardBaseTexture,GetCardImage, GetStage, GetCardPlacementSlots } from './main.js';
import { GetMousePosition } from './input.js';
import { Vector2 } from './types.js';

let cardTextStyle = new TextStyle({
	fontFamily: 'Arial',
	fontSize: 24,
	fill: '#ffffff',
});

export class Card {
	constructor(cardId) {
		let card = GetJsonCard(cardId);

		
		this.container = new Container();
		let scale = 1.5;
		const cardBase = new Sprite(GetCardBaseTexture());
		cardBase.anchor.set(0.5);
		cardBase.scale = scale;
		this.container.addChild(cardBase);	
		
		const character = new Sprite(GetCardImage(card.image));
		character.anchor.set(0.5);
		character.scale = card.imageScale;
		character.x = card.imageOffsetX;
		character.y = card.imageOffsetY;
		this.container.addChild(character);
		{
			cardTextStyle.fill = 'white';
			const richText = new Text({
				text: card.name,
			});
			richText.style = cardTextStyle;
			richText.anchor.set(0.5);
			richText.scale = scale *0.45;
			richText.y = 4;
			this.container.addChild(richText);
		}
		
		{
		
			this.hpText = new Text({
				text: card.health,
			});
			this.hpText.style = cardTextStyle;
			this.hpText.anchor.set(0.5);
			this.hpText.scale = scale *0.6;
			this.hpText.y = -76;
			this.hpText.x = 55;
			this.container.addChild(this.hpText);
		}
		
		{
		
			this.attackText = new Text({
				text: card.attack,
			});
			this.attackText.style = cardTextStyle;
			this.attackText.anchor.set(0.5);
			this.attackText.scale = scale *0.6;
			this.attackText.y = -76;
			this.attackText.x = -54;
			this.container.addChild(this.attackText);
		}
		

		cardBase.anchor.set(0.5);

		this.isBeingDragged = false;
		this.hasBeenPlaced = false;
		this.targetPosition = new Vector2(0, 0);

		this.container.interactive = true;
        this.container.buttonMode = true; // Show a pointer cursor when hovering

		this.container
		.on('pointerdown', this.onDragStart.bind(this))
		.on('pointermove', this.onDragMove.bind(this))
		.on('pointerup', this.onDragEnd.bind(this))
		.on('pointerupoutside', this.onDragEnd.bind(this));

		this.currentDragOffset = new Vector2(0, 0);

		GetStage().addChild(this.container);
	}


	onDragStart(event) {
        this.isBeingDragged = true;
		this.currentDragOffset.x = this.container.x - GetMousePosition().x;
		this.currentDragOffset.y = this.container.y - GetMousePosition().y;
    }

    onDragMove() {
        if (!this.isBeingDragged) return;

    }

    onDragEnd() {
        this.isBeingDragged = false;


		let mousePosition = GetMousePosition();
		if(this.hasBeenPlaced == false)
		{

			let cardPlacementSlots = GetCardPlacementSlots();

			for (let i = 0; i < cardPlacementSlots.length; i++) 
			{
				cardPlacementSlots[i].visible = true;
			
				const spriteBounds = cardPlacementSlots[i].getBounds();
			
				if (mousePosition.x > spriteBounds.x &&
					mousePosition.x < spriteBounds.x + spriteBounds.width &&
					mousePosition.y > spriteBounds.y &&
					mousePosition.y < spriteBounds.y + spriteBounds.height) 
				{
					this.hasBeenPlaced = true;
					this.targetPosition.x = spriteBounds.x + spriteBounds.width * 0.5;
					this.targetPosition.y = spriteBounds.y + spriteBounds.height * 0.5;
				}
			}
		}
    }

	Update(deltaTime, handIndex)
	{
		let moveToHandSpeed = 0.1;
		if(this.isBeingDragged)
		{
			let xVelocity = ( GetMousePosition().x + this.currentDragOffset.x - this.container.x) * deltaTime;
			this.container.angle = xVelocity;
			this.container.x += xVelocity;
			this.container.y += ( GetMousePosition().y + this.currentDragOffset.y - this.container.y) * deltaTime;
		}
		else if (this.hasBeenPlaced)
		{
			this.container.angle -= (this.container.angle) * deltaTime*moveToHandSpeed;
			this.container.x = LerpAndSnapIfUnder(this.container.x, this.targetPosition.x,1,deltaTime * moveToHandSpeed);
			this.container.y = LerpAndSnapIfUnder(this.container.y, this.targetPosition.y,1,deltaTime * moveToHandSpeed);
		}
		else
		{
			let handX = 100 + 170 * handIndex;
			let handY = window.innerHeight - 170;

			this.container.angle -= (this.container.angle) * deltaTime*moveToHandSpeed;

			this.container.x = LerpAndSnapIfUnder(this.container.x, handX,1,deltaTime * moveToHandSpeed);
			this.container.y = LerpAndSnapIfUnder(this.container.y, handY,1,deltaTime * moveToHandSpeed);

			//this.container.x += (handX - this.container.x) * deltaTime*moveToHandSpeed;
			//this.container.y += (handY - this.container.y) * deltaTime*moveToHandSpeed;
		}

	}

}


function LerpAndSnapIfUnder(current, target, thresholdDistanceToSnap, deltaTime)
{
	if (Math.abs(current - target) < thresholdDistanceToSnap)
	{
		return target;
	}

	return (1 - deltaTime) * current + deltaTime * target
}



async function GetTexture(textureName) 
{
	let texture = await Assets.load("./assets/cards/" + textureName);
	texture.source.scaleMode = 'nearest';
	return texture;
}