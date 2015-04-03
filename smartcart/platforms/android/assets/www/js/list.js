var isLogoutDialogOpen = false;

$(document).ready(function () {
	if(session_get("isLoggedIn") !== "true") {
		window.stop();
		window.location.href = "index.html";
	} 
	session_store("test", "testing");
});

// custom transformation: scale header's title
    var titleStyle = document.querySelector('.title').style;
    addEventListener('core-header-transform', function(e) {
      var d = e.detail;
      var m = d.height - d.condensedHeight;
      var scale = Math.max(0.75, (m - d.y) / (m / 0.25)  + 0.75);
      titleStyle.transform = titleStyle.webkitTransform =
          'scale(' + scale + ') translateZ(0)';
    });

$("core-icon-button[icon='settings-power']").click(function () {
	
	if(session_get("isLoggedIn") == "true") {
		if(!isLogoutDialogOpen) {
			$(".logoutConfirmationDialog").addClass("active");
			isLogoutDialogOpen = true;
		}
	}

	return false;
});

$("a#logoutDecline").click(function () {

	if(isLogoutDialogOpen) {
		$(".logoutConfirmationDialog").removeClass("active");
		isLogoutDialogOpen = false;
	}

	return false;
});

$("a#logoutAccept").click(function () {

	if(isLogoutDialogOpen) {
		$(".logoutConfirmationDialog").removeClass("active");
		isLogoutDialogOpen = false;
		session_remove("isLoggedIn");
		session_remove("username");
		setTimeout(function () {
			window.location.href = "index.html";
		}, 500);
	}

	return false;
});

$("#cartCheckout").click(function () {

	window.location.href = "checkout.html";

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

function session_remove(key) {
	if(typeof(Storage) != "undefined") {
		if(localStorage.getItem(key)) {
			return localStorage.removeItem(key);
		} else {
			console.log("[DEBUG]: Storage not supported")
		}
	}
	return false;
}