/*
 *  @(#)common.js  last: 13.02.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

let bright = 3.3;

const getBright = () => {
  fetch('http://192.168.0.100/api/v1/bright', {
    method: 'GET',
    headers: {
      'Accept': 'application/json'
    }
  })
    .then(response => response.json())
    .then(response => {
      console.log(JSON.stringify(response));
      setBright(response.bright);
    })
    .catch(err => {
      console.log(err);
    })
}

const setBright = (volt) => {
  bright = volt;
  const elm = document.getElementById("bright");
  elm.textContent = bright.toFixed(2);
};
