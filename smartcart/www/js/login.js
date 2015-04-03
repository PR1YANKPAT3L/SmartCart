var tabs = document.querySelector('paper-tabs'),
	pages = document.querySelector('core-pages'),
	loginBtn = $("paper-button[name='login-signin']"),
	userError = document.querySelector('#userError'),
	passError = document.querySelector('#passError'),
	loginError = document.querySelector('#loginError'),
	loginSuccess = document.querySelector('#loginSuccess'),
	loginSpinner = document.querySelector("paper-spinner");

$(document).ready(function () {
	if(session_get("isLoggedIn") == "true") {
		window.stop();
		window.location.href = "list.html";
	} 
});

tabs.addEventListener('core-select',function() {
	pages.selected = tabs.selected;
});

loginBtn.click(function () {
	loginSpinner.active = true;
	var user = $("input[name='login-name']").val(),
		pass = $("input[name='login-pass']").val();

	if(user == '' && user.length == 0) {
		userError.show();
		setTimeout(function () {
			loginSpinner.active = false;
		}, 500);
	} else if(pass == '' && pass.length == 0) {
		passError.show();
		setTimeout(function () {
			loginSpinner.active = false;
		}, 500);
	} else {
		$.ajax({
			url: 'http://smartcart.x10.mx/public/v1/user/' + user + '/' + pass,
			type: 'POST',
			data: '',
			success: function (data) {
				if(data && data.success) {
					loginSuccess.setAttribute("text", "Welcome, " + data.success.username + ". Please wait...");
					loginSuccess.show();
					setTimeout(function () {
						session_store("isLoggedIn", "true");
						session_store("username", "silentcoder");
						setTimeout(function () {
							loginSpinner.active = false;
						}, 1000);
						document.location.href = 'list.html';
					}, 800);
				} else {
					loginError.show();
					setTimeout(function () {
						loginSpinner.active = false;
					}, 800);
				}
			},
			error: function(xhr, textStatus, errorthrown) {
				console.log(textStatus);
			}
		});
	}

	return false;
});

function session_store(key, value) {
	if(typeof(Storage) !== "undefined") {
		localStorage.setItem(key, value);
	} else {
		console.log("[DEBUG]: Storage not supported");
	}
}

function session_get(key) {
	if(typeof(Storage) !== "undefined") {
		if(localStorage.getItem(key)) {
			return localStorage.getItem(key);
		} else {
			console.log("[DEBUG]: Session not found!");
		}
	} else {
		console.log("[DEBUG]: Storage not supported")
	}
}