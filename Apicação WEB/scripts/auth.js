document.addEventListener("DOMContentLoaded", function(){
    var UserState; 
    // Espera por mudanças no estado de autenticação
    auth.onAuthStateChanged(user => {
        //carrega nova página e muda o estado do usuário
        if (user) {
          setupUI(user); 
          var uid = user.uid;
          UserState = db.ref('UsersData/' + uid.toString()+ '/Estado');
          UserState.set({ativo : true});
        } else {
          setupUI();
        }
    });

    // formulario de login e autentificação do usuário
    const loginForm = document.querySelector('#login-form');
    loginForm.addEventListener('submit', (e) => {
        e.preventDefault();
        const email = loginForm['input-email'].value;
        const password = loginForm['input-password'].value;
        auth.signInWithEmailAndPassword(email, password).then((cred) => {
            loginForm.reset();
            console.log(email);
        })
        .catch((error) =>{
            const errorCode = error.code;
            const errorMessage = error.message;
            document.getElementById("error-message").innerHTML = errorMessage;
            console.log(errorMessage);
        });
    });

    // logout
    const logout = document.querySelector('#logout-link');
    logout.addEventListener('click', (e) => {
        e.preventDefault();
        UserState.set({ativo : false});
        auth.signOut();
    });
});  