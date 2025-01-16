import { Application, Assets, Sprite,Text, TextStyle, Container, Color, BlurFilter } from 'https://cdn.skypack.dev/pixi.js';
import { SendMessage, InitClient, IsConnected } from './client.js';
import { RegisterDragItem, HandleDragItem } from './dragItem.js';
import { OnMouseMove, GetMousePosition } from './input.js';
import { Vector2 } from './types.js';
import { Card } from './HandCard.js';

let jsonCards={};
fetch('../Cards.json')
    .then((response) => response.json())
    .then((json) => jsonCards = json);



let cardImageMap = new Map();

let cardBaseTexture;
let cardBackTexture;
let characterSlotTexture;
let app;


const hand = [];
const cardPlacementSlots = [];


(async () =>
{
    app = new Application();
    await app.init({
      	width: 1280, 
      	height: 900, 
      	backgroundColor: 0x1099bb,
      	antialias: false
    });



    document.body.appendChild(app.canvas);
    const mainMenuContainer = document.getElementById('mainMenuContainer');
    const loginContainer = document.getElementById('loginContainer');
    const inputField = document.getElementById('myInput');
    const loginButton = document.getElementById('loginButton');
    const findMatchButton = document.getElementById('findMatchButton');
	document.body.appendChild(loginContainer);

    loginButton.addEventListener("click", function() {
		InitClient().then(function() {
          	const jsonData = {
           		username: inputField.value
          	};
          	SendMessage('LoginRequest', jsonData);
          	loginContainer.style.display = 'none';
          	mainMenuContainer.style.display = 'block';
			
        }).catch(function(err) {
            // error
      });
    });    
	findMatchButton.addEventListener("click", function() {
		findMatchButton.style.display = 'none';
		document.getElementById('lookingForGameText').style.display = 'block';
		const jsonData = {};
		SendMessage('FindGame', jsonData);
    });
    document.body.appendChild(mainMenuContainer);

    



    cardBaseTexture = await Assets.load("./assets/CardBase.png");
    cardBaseTexture.source.scaleMode = 'nearest';
	
    cardBackTexture = await Assets.load("./assets/CardBack.png");
    cardBackTexture.source.scaleMode = 'nearest';    
	
	characterSlotTexture = await Assets.load("./assets/CharacterSlot.png");
    characterSlotTexture.source.scaleMode = 'nearest';

    const backgroundTexture = await Assets.load("./assets/Background.png");
    const background = new Sprite(backgroundTexture);
    background.anchor.set(0.5);
    background.x = 1280 / 2;
    background.y = 720 / 2 + 90;
	background.scale  = 1.15;
    background.eventMode = 'static';
    background.filters = new BlurFilter({ strength: 8 });
    //background.on('pointerdown', (event) => {
    //  const elem = document.documentElement;
    //  if (elem.requestFullscreen) {
    //    elem.requestFullscreen();
    //  } else if (elem.mozRequestFullScreen) { // For Firefox
    //      elem.mozRequestFullScreen();
    //  } else if (elem.webkitRequestFullscreen) { // For Chrome, Safari, and Opera
    //      elem.webkitRequestFullscreen();
    //  } else if (elem.msRequestFullscreen) { // For IE/Edge
    //      elem.msRequestFullscreen();
    //  }
    //  app.renderer.resize(window.innerWidth, window.innerHeight);
    //  background.x = window.innerWidth / 2;
    //  background.y = window.innerHeight / 2;
    //});
    app.stage.addChild(background);
    app.stage.eventMode = 'static';
	CreateCharacterSlot(300,580);
	CreateCharacterSlot(450,580);
	CreateCharacterSlot(600,580);
	CreateCharacterSlot(750,580);
	CreateCharacterSlot(900,580);

	for (let card of jsonCards.cards) 
	{
		let texture = await Assets.load("./assets/cards/" + card.image);
		texture.source.scaleMode = 'nearest';
		cardImageMap.set(card.image, texture);
	}

	//hand.push(new Card("skeleton"));
	//hand.push(new Card("cultist"));
	//hand.push(new Card("angrygoblin"));


    app.stage.addEventListener('mousemove', OnMouseMove);
    loginContainer.style.display = 'block';
    app.ticker.add((time) =>
    {
    	const rect = app.canvas.getBoundingClientRect();
		loginContainer.style.top = rect.top + rect.height * 0.5 + "px";  // Offset by 50 pixels from the top of the canvas
		loginContainer.style.left = rect.left + rect.width * 0.5 - loginContainer.getBoundingClientRect().width * 0.5 + "px";  // Offset by 100 pixels from the left of the canvas
		mainMenuContainer.style.top = rect.top + rect.height * 0.5 + "px";  // Offset by 50 pixels from the top of the canvas
		mainMenuContainer.style.left = rect.left + rect.width * 0.5 - mainMenuContainer.getBoundingClientRect().width * 0.5 + "px";  // Offset by 100 pixels from the left of the canvas
  
		let isDraggingAnyCard = false;

		for (let i = 0; i < hand.length; i++) 
		{
			hand[i].Update(time.deltaTime,i);
			if(hand[i].isBeingDragged)
			{
				isDraggingAnyCard = true;
			}
		}

		if(isDraggingAnyCard)
		{
			for (let i = 0; i < cardPlacementSlots.length; i++) 
			{
				cardPlacementSlots[i].visible = true;
			}
		}
		else
		{
			for (let i = 0; i < cardPlacementSlots.length; i++) 
			{
				cardPlacementSlots[i].visible = false;
			}
		}

		HandleDragItem(time.deltaTime);
      
		if(IsConnected())
		{
			SendMessage("memes tho");
		}
	});
})();


function CreateCharacterSlot(x,y)
{

	let container = new Container();
	let scale = 2.0;
	const texture = new Sprite(characterSlotTexture);
	texture.anchor.set(0.5);
	texture.scale = scale;
	container.addChild(texture);
	
	container.x = x;
	container.y = y;
	
	container.interactive = true;
	container.visible = true;
	app.stage.addChild(container);
	cardPlacementSlots.push(container);
}


let slot = 1;
let enemySlot = 1;

export function GetCardPlacementSlots()
{
	return cardPlacementSlots;
}

export function OnGameFound()
{
	document.getElementById('lookingForGameText').style.display = 'none';
	console.log("Match Found!");
}


export function GetStage()
{
	return app.stage;
}

export function GetCardImage(image)
{
	return cardImageMap.get(image);
}


export function GetCardBaseTexture()
{
	return cardBaseTexture;
}

export function GetJsonCard(cardId)
{
	for (let element of jsonCards.cards) {
		
		if(element.id == cardId)
		{
			return element;
		}
	}
}


export async function AddCard(cardId)
{
	if(cardId.isThisUsersCard == false)
	{

		let cardContainer = new Container();
		let scale = 1.5;
		const cardBack = new Sprite(cardBackTexture);
		cardBack.anchor.set(0.5);
		cardBack.scale = scale;
		cardContainer.addChild(cardBack);	

		app.stage.addChild(cardContainer);
	
		cardContainer.x = app.screen.width - 600 + 110 * enemySlot * scale ;
		cardContainer.y = 100;
		cardBack.anchor.set(0.5);

		enemySlot++;
		return cardContainer;
	}

	hand.push(new Card(cardId.id));
}
