let bright = 3.3;
// let baseUrl = "http://192.168.0.100/";
let baseUrl = "http://localhost:5000/";
let dateTime = {
  year: 2024, month: 13, day: 27,
  hour: 1, min: 59, sec: 59
};
let dateTimeInp = {};

const getBright = () => {
  fetch(baseUrl + "api/v1/bright", {
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
};

const setBright = (volt) => {
  bright = volt;
  const elm = document.getElementById("bright");
  elm.textContent = bright.toFixed(2);
};

const getDatetime = () => {
  fetch(baseUrl + "api/v1/datetime", {
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
};

const postDatetime = () => {
  fetch(baseUrl + "api/v1/datetime", {
    method: "POST",
    headers: {
      "Content-type": "application/json",
      "Accept": "application/json"
    },
    date: dateTime
  })
    .then(response => response.json())
    .then(response => {
      console.log(JSON.stringify(response));
      dateTime = response;
    })
    .catch(err => {
      console.log(err);
    });
};
