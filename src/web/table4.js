$(document).ready(function () {
let a = 1;
});

document.addEventListener("DOMContentLoaded", function () {
  // Your code goes here
  let t1 = new Table({name: "t1", div_id: "d1", id: "t1"});
  t1.add_row({id: "t1.r1", text: "t1.r1"});
  t1.add_th({row_id: "t1.r1", id: "t1.r1.h1", text: "t1.r1.h1"});
  t1.add_row({id: "t1.r2", text: "t1.r2"});
  t1.add_td({row_id: "t1.r2", id: "t1.r2.d1", text: "t1.r2.d1"});
  t1.get_th({row_id: "t1.r1", id: "t1.r1.h1"})
    .addEventListener('contextmenu', function (e) {
    e.preventDefault();
    alert("t1.r1.h1 right-click!");
  }, false);

  t1.get_td({row_id: "t1.r2", id: "t1.r2.d1"})
  .addEventListener('click', function (e) {
    e.preventDefault();
    alert("t1.r1.d1 click!");
  }, false);

});
