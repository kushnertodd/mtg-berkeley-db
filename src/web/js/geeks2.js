
$("#h11").html("ready!");
$(document).ready(function() {
  $("button").click(function() {
    //$.ajax({url: "geeks.txt",
    //$.ajax({url: "http://cryptogram.org",
    //$.ajax({url: "http://cryptogram.org",
    //$.ajax({url: "http://127.0.0.1:8000/hello",
    $.ajax({url: "http://127.0.0.1:8000/mtg-request-dto",
            type: "POST",
            data: '{"class_name":"Mtg_request","request":"load_name","arguments":["../../data/tsv-sample/name.basics.tsv"]}',
    success: function(result) {
      //$("#h11").html(result);
      var data = JSON.stringify(result);
      $("#h11")[0].innerHTML = data;
      //let result1 = document.querySelector('.result1');
      //result1.innerHTML = data;
      $(".result1")[0].innerHTML = data;
      //let result2 = document.querySelector('#result2');
      //result2.innerHTML = data;
      $("#result2")[0].innerHTML = data;
      let result5 = JSON.parse(data);
      let result6 = JSON.parse(result2.message);
      console.log(result);
    }});
  });
});
