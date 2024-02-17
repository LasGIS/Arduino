let baseUrl = "http://192.168.0.100";
//let baseUrl = "http://localhost:5000";

const setBright = (volt) => {
  const bright = volt;
  const element = document.getElementById("bright");
  element.textContent = bright.toFixed(2);
}

const setDatetime = (datetime) => {
  const twoDigits = (value) => Number(value).toLocaleString("ru-RU", { minimumIntegerDigits: 2 });
  const str = ''
    + twoDigits(datetime.hour) + ':'
    + twoDigits(datetime.minute) + ':'
    + twoDigits(datetime.second) + ' - '
    + twoDigits(datetime.day) + '.'
    + twoDigits(datetime.month) + '.'
    + datetime.year;
  const element = document.getElementById("data-time");
  element.textContent = str;
}

const getBright = () => {
  fetch(baseUrl + "/api/v1/bright", {
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
  fetch(baseUrl + "/api/v1/datetime", {
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
  fetch(baseUrl + "/api/v1/datetime", {
    method: "POST",
    headers: {
      "Accept": "application/json",
      "Content-type": "application/x-www-form-urlencoded"
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
  const date = new Date();
  let dateTime = {
    year: date.getFullYear(),
    month: date.getMonth(),
    day: date.getDate(),
    hour: date.getHours(),
    minute: date.getMinutes(),
    second: date.getSeconds()
  };
  postDatetime(dateTime);
}
