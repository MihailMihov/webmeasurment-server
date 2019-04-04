function loadFile(filepath) {

    var fileRead = new XMLHttpRequest();

    fileRead.onreadystatechange = function() {

        if (this.readyState == 4 && this.status == 200) {

            dataArray = parseFile(this.responseText);
            google.charts.load('current', { 'packages': ['line', 'corechart'] });

        }

    };

    xmlhttp.open("GET", filepath, true);
    xmlhttp.send();

}

function parseFile(text) {

  var array = [];
  var lines = string.split("\n");

  for (var i = 0; i < lines.length; i++) {

      var data = lines[i].split(",", 2);
      data[0] = new Date(parseInt(data[0]) * 1000);
      data[1] = parseFloat(data[1]);
      array.push(data);

  }

  return array;


}
