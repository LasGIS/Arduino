let bright = 3.3;
const setBright = (volt) => {
  bright = volt;
  const elm = document.getElementById("bright");
  elm.textContent = bright.toFixed(2);
};
