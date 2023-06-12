function epochToJsDate(epochTime){
    return new Date(epochTime*1000);
  }
  
  // converte epochtime para data e hora
  function epochToDateTime(epochTime){
    var epochDate = new Date(epochToJsDate(epochTime));
    var dateTime = ("00" + epochDate.getDate()).slice(-2) + "/" +
      ("00" + (epochDate.getMonth() + 1)).slice(-2) + "/" +
      epochDate.getFullYear() + " " +
      ("00" + epochDate.getHours()).slice(-2) + ":" +
      ("00" + epochDate.getMinutes()).slice(-2) + ":" +
      ("00" + epochDate.getSeconds()).slice(-2);
      
    return dateTime;
  }

  const loginElement = document.querySelector('#login-form');
  const contentElement = document.querySelector("#content-sign-in");
  const userDetailsElement = document.querySelector('#user-details');
  const authBarElement = document.querySelector('#authentication-bar');
  
  // Escutando para mudanças de estado de autenticação
  const setupUI = (user) => {
    //Estado em que o usuário está logado	
    if (user) {
      loginElement.style.display = 'none';
      contentElement.style.display = 'block';
      authBarElement.style.display ='block';
      userDetailsElement.style.display ='block';
      userDetailsElement.innerHTML = user.email;
  
      var uid = user.uid;
      var dbPath = 'UsersData/' + uid.toString() + '/SHT';
      var dbRef = firebase.database().ref(dbPath);

      //Objeto referente ao sensor de temperatura e umidade
      let SHT = {
        'databaseSHT': db.ref('UsersData/' + uid.toString() + '/SHT/'),
        'databaseSHT_request': db.ref('UsersData/' + uid.toString() + '/Request/SHT_request'),
        'request' : true,
        read_data : function(){
          this.databaseSHT.orderByKey().limitToLast(1).on('child_added', snapshot =>{
            let jsonData = snapshot.toJSON(); // example: {temperature: 25.02, humidity: 50.20, pressure: 1008.48, timestamp:1641317355}
            let tempC = jsonData.TemperaturaC;
            document.getElementById("SHT_tempC").innerHTML = tempC + "°C";
            let tempF = jsonData.TemperaturaF;
            document.getElementById("SHT_tempF").innerHTML = tempF + "°F";
            let hum = jsonData.umidade;
            document.getElementById("SHT_umidade").innerHTML = hum + "%";
            let tempo = jsonData.timestamp;
            document.getElementById("SHT_data").innerHTML = epochToDateTime(tempo);
          });
        },
        getData: function() {
          this.databaseSHT_request.set({SHT_request : this.request});
        }
      }

      //Botão para solicitar dados do sensor de temperatura e umidade
      const SHT_button = document.getElementById('SHT_button');
      if(SHT_button)
      SHT_button.addEventListener('click', function() {
        SHT.getData();
      });
      setInterval(function () {SHT.getData()}, 500000);

      //Objeto referente ao sensor de luminosidade
      let Lightsensor = {
        'databaseLight': db.ref('UsersData/' + uid.toString() + '/BHT1750/'),
        'databaseLight_request': db.ref('UsersData/' + uid.toString() + '/Request/BH1750_request'),
        'request' : true,
        read_data : function(){
          this.databaseLight.orderByKey().limitToLast(1).on('child_added', snapshot => {
            let jsonData = snapshot.toJSON();
            let lum = jsonData.Lux;
            document.getElementById("Lux").innerHTML = lum + "Lux";
            let tempo = jsonData.timestamp;
            document.getElementById("Light_data").innerHTML = epochToDateTime(tempo);

            if (parseInt(lum) < 100) {
              document.body.style.backgroundImage = "url('https://images.unsplash.com/photo-1503428009254-8d4f29217f23?ixlib=rb-4.0.3&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=870&q=80')";
            } else {
              document.body.style.backgroundImage = "url('https://images.unsplash.com/photo-1563514227147-6d2ff665a6a0?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=1471&q=80')";
            }
          })
        }, 
        getData: function() {
          this.databaseLight_request.set({BH1750_request : this.request});
        }
      }

      //Botão para solicitar dados do sensor de luminosidade
      const BH1750_button = document.getElementById('BH1750_button');
      if(BH1750_button)
      BH1750_button.addEventListener('click', function() {
        Lightsensor.getData();
      });
      setInterval(function () {Lightsensor.getData()}, 500000);

      //Objeto referente a api do openweather
      let meteorologia = {
        "apiKey": "323f89ded91e4207073da612278d6ad0",
        "databaseGPS": db.ref('UsersData/' + uid.toString() + '/GPS/'),
        read_GPS: function() {
          this.databaseGPS.orderByKey().limitToLast(1).on('child_added', snapshot =>{
            let jsonData = snapshot.toJSON();
            console.log(jsonData);
            let lat = jsonData.LAT;
            let lng = jsonData.LNG;
            this.fetchWeather(lat, lng);
          });
        },
        fetchWeather: function(latitude, longitude) {
            fetch("https://api.openweathermap.org/data/2.5/weather?lat=" +latitude+"&lon="+longitude+"&units=metric&lang=pt&appid=" + this.apiKey
            ).then((response) => response.json())
            .then((data) => this.displayWeather(data));
        },
        displayWeather: function(data) {
            const {name} = data;
            const {icon, description} = data.weather[0];
            const {temp, humidity} = data.main;
            const {speed} = data.wind
      
            const {sunrise} = data.sys;
            let nascente = new Date(0);
            nascente.setUTCSeconds(sunrise);
            let nascer = nascente.toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'});
      
            const {sunset} = data.sys;
            let poente = new Date(0);
            poente.setUTCSeconds(sunset);
            let por = poente.toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'});
      
           // console.log(name, icon, description, temp, humidity, speed);
            document.querySelector('.cidade').innerText = "Clima em " + name;
            document.querySelector('.icon').src = "http://openweathermap.org/img/wn/"+icon+".png";
            document.querySelector('.descricao').innerText = description.charAt(0).toUpperCase() + description.slice(1).toLowerCase();;
            document.querySelector('.temp').innerText = temp + '°C';
            document.querySelector('.umidadeSat').innerText = "Umidade: " + humidity + "%";
            document.querySelector('.vento').innerText = "Vento: " + speed + " km/h";
            document.querySelector('.nascer').innerText = "Nascer do sol: " + nascer;
            document.querySelector('.por').innerText = "Por do sol: " + por;
        },
      }

      //Objeto referente a api do google maps
      let localizacao = {
        "databaseGPS": db.ref('UsersData/' + uid.toString() + '/GPS/'),
        read_GPS: function() {
          this.databaseGPS.orderByKey().limitToLast(1).on('child_added', snapshot =>{
            let jsonData = snapshot.toJSON();
            console.log(jsonData);
            let lat = parseFloat(jsonData.LAT);
            let lng = parseFloat(jsonData.LNG);
            this.initMap(lat, lng);
            
          })
        },
        initMap: function(lat, lng){
        var myLatLng = {lat: lat, lng: lng}; 
        var map = new google.maps.Map(document.querySelector('.map'), {
          zoom: 12,
          center: myLatLng
        });
        
        var marker = new google.maps.Marker({
          position: myLatLng,
          map: map,
          title: 'Meu local'
        });
        }
      }

      //Data min e data max do dashboard
      const datmin = document.querySelector('#datemin');
      const datemax = document.querySelector('#datemax');
      const submit = document.querySelector('#submit');

      const temperaturaGraf = document.querySelector('#temperaturaGraf');
      const umidadeGraf = document.querySelector('#umidadeGraf');

      //Atualização dos gráficos
      if(submit)
      submit.addEventListener('click', function() {
        const medida = document.getElementById('medida').value;
        const resolucao = document.getElementById('resolucao').value;
        temperaturaGraf.setAttribute('src', "https://agriot-9b824.rj.r.appspot.com/plots/temp?datemin="+datmin.value+"&datemax="+datemax.value+"&medida="+medida+"&resolucao="+resolucao);
        umidadeGraf.setAttribute('src', "https://agriot-9b824.rj.r.appspot.com/plots/umid?datemin="+datmin.value+"&datemax="+datemax.value)
      });

      //Download dos dados
      const download = document.querySelector('#download');
      if(download)
      download.addEventListener('click', function() {
        const tipo = document.getElementById('tipo').value;
      const periodo = document.getElementById('periodo').value;
        window.open("https://agriot-9b824.rj.r.appspot.com/plots/download?formato="+tipo+'&periodo='+periodo);
      });
      
      //Requisição das leituras 
      SHT.read_data();
      Lightsensor.read_data();
      meteorologia.read_GPS();
      localizacao.read_GPS();
  
    // Estado em que o usuário fez log out
    } else{
      loginElement.style.display = 'block';
      authBarElement.style.display ='none';
      userDetailsElement.style.display ='none';
      contentElement.style.display = 'none';
    }
  }