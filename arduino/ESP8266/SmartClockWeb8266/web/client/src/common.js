//let baseUrl = "http://192.168.0.103";
let baseUrl = "http://localhost:5000";

const setBright = (volt) => {
  const bright = volt;
  const element = document.getElementById("bright");
  element.textContent = bright.toFixed(2);
};

const setDatetime = (datetime) => {
  const twoDigits = (value) => Number(value).toLocaleString("ru-RU", { minimumIntegerDigits: 2 });
  const elementTime = document.getElementById("control-time");
  elementTime.textContent = ""
    + twoDigits(datetime.hour) + ":"
    + twoDigits(datetime.minute) + ":"
    + twoDigits(datetime.second);
  const elementData = document.getElementById("control-data");
  elementData.textContent = ""
    + twoDigits(datetime.day) + "."
    + twoDigits(datetime.month) + "."
    + datetime.year;
  const elementDayOfWeek = document.getElementById("day-of-week");
  elementDayOfWeek.selectedIndex = datetime.dayOfWeek;
};

const getBright = () => {
  fetch(baseUrl + "/api/v1/bright", {
    method: "GET",
    headers: {
      "Accept": "application/json"
    }
  })
    .then(response => response.json())
    .then(response => {
      setBright(response.bright);
    })
    .catch(err => {
      console.log(err);
    });
};

const getDatetime = () => {
  fetch(baseUrl + "/api/v1/datetime", {
    method: "GET",
    headers: {
      "Accept": "application/json"
    }
  })
    .then(response => response.json())
    .then(datetime => {
      setDatetime(datetime);
    })
    .catch(err => {
      console.log(err);
    });
};

const postDatetime = (dateTime) => {
  let data = JSON.stringify(dateTime);
  fetch(baseUrl + "/api/v1/datetime", {
    method: "POST",
    headers: {
      "Accept": "application/json",
      "Content-type": "application/json"
    },
    body: data
  })
    .then(response => response.json())
    .then(datetime => {
      setDatetime(datetime);
    })
    .catch(err => {
      console.log(err);
    });
};

const getScanNetworks = (resolve) => {
  fetch(baseUrl + "/api/v1/scan-networks", {
    method: "GET",
    headers: {
      "Accept": "application/json"
    }
  })
    .then(response => response.json())
    .then(scanNetworks => resolve(scanNetworks))
    .catch(err => {
      console.log(err);
    });
};

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
};

const showScanNetworks = () => {
  getScanNetworks(scanNetworks => {
    const value = JSON.stringify(scanNetworks, null, '  ');
    console.log(value);
    const scanWifi = document.getElementById("scan-wifi");
    scanWifi.value = value;
  });
};

const jsonRpc = () => {
  fetch("https://localhost:7239/api/jsonrpc", {
    method: "POST",
//    mode: "no-cors",
    headers: {
      "Accept": "application/json",
      "Accept-Encoding": "br",
      "Content-Type": "application/json"
    },
    body: JSON.stringify({
      id: 1,
      jsonrpc: "2.0",
      method: "delta_mod.get_property_command",
      params: {
        property_name: "LoadedSimCasesInfo",
        path: [],
        args: []
      }
    })
  })
    .then(response => {
      console.log(response.status);
      return response.text();
    })
    .then(json => {
      console.log(json);
    })
    .catch(err => {
      console.log(err);
    });
};
