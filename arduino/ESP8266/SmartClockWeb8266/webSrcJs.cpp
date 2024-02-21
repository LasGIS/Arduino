#include "SmartClockWeb8266.h"

const char srcCommonJs[] = R"=====(// Common JS
const setBright = (volt) => {
  const bright = volt;
  const element = document.getElementById("bright");
  element.textContent = bright.toFixed(2);
}

const setDatetime = (datetime) => {
  const twoDigits = (value) => Number(value).toLocaleString("ru-RU", { minimumIntegerDigits: 2 });
  const elementTime = document.getElementById("control-time");
  elementTime.textContent = ''
    + twoDigits(datetime.hour) + ':'
    + twoDigits(datetime.minute) + ':'
    + twoDigits(datetime.second);
  const elementData = document.getElementById("control-data");
  elementData.textContent = ''
    + twoDigits(datetime.day) + '.'
    + twoDigits(datetime.month) + '.'
    + datetime.year;
  const elementDayOfWeek = document.getElementById("day-of-week");
  elementDayOfWeek.selectedIndex = datetime.dayOfWeek;
}

const getBright = () => {
  fetch("/api/v1/bright", {
    method: "GET",
    headers: {
      "Accept": "application/json"
    }
  })
    .then(response => response.json())
    .then(response => {
      console.log(JSON.stringify(response));
      setBright(response.bright);
    })
    .catch(err => {
      console.log(err);
    });
}

const getDatetime = () => {
  fetch("/api/v1/datetime", {
    method: "GET",
    headers: {
      "Accept": "application/json"
    }
  })
    .then(response => response.json())
    .then(datetime => {
      console.log(JSON.stringify(datetime));
      setDatetime(datetime);
    })
    .catch(err => {
      console.log(err);
    });
}

const postDatetime = (dateTime) => {
  let data = JSON.stringify(dateTime);
  fetch("/api/v1/datetime", {
    method: "POST",
    headers: {
      "Content-type": "application/json",
      "Accept": "application/json",
    },
    body: data
  })
    .then(response => response.json())
    .then(datetime => {
      console.log(JSON.stringify(datetime));
      setDatetime(datetime);
    })
    .catch(err => {
      console.log(err);
    });
}

const synchroDatetime = () => {
  const elementDayOfWeek = document.getElementById("day-of-week");
  const date = new Date();
  postDatetime({
    year: date.getFullYear(),
    month: date.getMonth() + 1,
    day: date.getDate(),
    hour: date.getHours(),
    minute: date.getMinutes(),
    second: date.getSeconds(),
    dayOfWeek: elementDayOfWeek.selectedIndex
  });
}
)=====";

void webGetSrcCommonJs() {
  server.send(200, "application/javascript", srcCommonJs);
}
