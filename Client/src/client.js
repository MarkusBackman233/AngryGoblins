let socket;
let connected = false;
import { AddCard, OnGameFound } from './main.js';

const syncMessageMap = new Map();

syncMessageMap.set("AddCard", AddCard);
syncMessageMap.set("OnGameFound", OnGameFound);

export function IsConnected()
{
    return connected;
}

export function InitClient()
{
    return new Promise(function(resolve, reject) {
        socket = new WebSocket('ws://localhost:8080');
        connected = false;


        socket.onopen = () => {
            console.log('WebSocket connection opened');
            resolve();
        };

        socket.onmessage = (event) => {
            const jsonObject = JSON.parse(event.data);
            syncMessageMap.get(jsonObject.type)(jsonObject);
        };

        socket.onerror = (error) => {
            console.error('WebSocket error:', error);
            reject(err);
        };

        socket.onclose = () => {
            console.log('WebSocket connection closed');
        };

    });
}

export function SendMessage(type, message) 
{
    const jsonData = {
        type: type,
        content: message
    };

    const jsonString = JSON.stringify(jsonData);

	socket.send(jsonString);
}
