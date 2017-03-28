var _getInProgress = false;
var _queueFunc = null;
var _queueURL = null;

// set timeout in case the server responds very slow or not at all
// also applies to GET requests, since they are build onto AJAX requests
$.ajaxSetup({timeout: 6000}); 

// refresh values from server if this IntervallTimer is active
var pollTimer; 

function pollAll() {
	$.get("/all", function( data ) {			
		var color = tinycolor("rgb("+data.red+","+data.green+","+data.blue);
		
		// only update the colorpicker if color has changed
		//if (color.toHexString() != $("#flat").spectrum('get').toHexString()) {
			$("#flat").spectrum("set", color);
			$("#flat").spectrum("reflow");
			$("#flat").spectrum("set", color);
			$("#flat").spectrum("show");
		//}

		var txtcolor = tinycolor.mostReadable(color, ["#afafaf", "#000"]);
		$("#cvalue,#current_color").css("background-color", color.toHexString());
		$("#cvalue,#current_color").css("color", txtcolor.toHexString());
		$("#cvalue,#current_color").text("R: "+data.red+", "+"G: "+data.green+", "+"B: "+data.blue);

		debug("Uptime: "+ data.uptime.toString());
		
		$("#current_state").text(data.state_human_readable);
	});
}

function debug(text) {
	$(".deb").text(text)
		.stop()
		.animate({opacity: '1'}, 100)
		.animate({opacity: '0'}, 4000);
}

function myGet(URL, func) {
	// if we are still busy (server did not respond yet), queue this new request, overwrite previously queued request
	if (_getInProgress) {
		_queueFunc = func, _queueURL = URL;
		console.log("kept the last request, GET still busy...");
		return;
	}

	console.log("requesting: "+URL);
	_getInProgress = true;
	
	// initiate communication with server via HTTP GET request
	var jqxhr = $.get(URL, function( data ) {
		// GET returns without error, execute passed function
		func(data);
		})
		.done(function() {
			console.log("AJAX returned without errors");
		})
		.fail(function() {
			console.log("AJAX returned with ERROR!");
		})
		.always(function() {
			console.log("AJAX returned");
			
			_getInProgress = false;
			
			// check if there is a queued request
			if (_queueURL != null && _queueFunc != null) {
				var _dequeueURL = _queueURL, _dequeueFunc = _queueFunc;
				_queueURL = null;
				_queueFunc = null;
				console.log("GET not busy anymore, dequeue");
				
				// recursive call
				myGet(_dequeueURL, _dequeueFunc);
			}
		});
}

$( document ).ready(function() {
	/* start a timer */
	pollTimer = setInterval(pollAll, 1000);

	/* init the color picker */
	$("#flat").spectrum({
		flat: true,
		showInput: false,
		showButtons: false,
		preferredFormat: "rgb",
		move: function(color) {
			var txtcolor = tinycolor.mostReadable(color, ["#afafaf", "#000"]);
			$("#cvalue").css("background-color", color.toHexString());
			$("#cvalue").css("color", txtcolor.toHexString());

			$("#cvalue").text("R: "+Math.round(color._r).toString()+", " +
				"G: "+Math.round(color._g).toString()+", " +
				"B: "+Math.round(color._b).toString()/*+", " +
				"W: "+Math.round(255-color._a*255).toString()*/ );

			myGet("/color?"+
				"red="+Math.round(color._r).toString()+"&" +
				"green="+Math.round(color._g).toString()+"&" +
				"blue="+Math.round(color._b).toString(), function( data ) {
					//$(".deb").text("GET done: "+JSON.stringify(data));
				});
		}
	});

	$('.sp-val,.sp-hue').on('touchstart mousedown', function(evt) { clearInterval(pollTimer); });
	$('.sp-val,.sp-hue').on('touchend mouseup', function(evt) { clearInterval(pollTimer); pollTimer = setInterval(pollAll, 1000); });
	
	$("#startupcolor").spectrum({
		preferredFormat: "hex",
		localStorageKey: "lyt8266",
		showPalette: true
	});
	
	$(document.body).on('pageshow', function(evt) {
		if( $.mobile.activePage.attr("id") != "config" ) {
			clearInterval(pollTimer);
			pollAll(); 
			pollTimer = setInterval(pollAll, 1000);
			return;
		}
	
		clearInterval(pollTimer);

		$("#configform input").prop("disabled", true);
		$("#configform input[type='radio']").checkboxradio("refresh");
		
		$.get("/config.json", function( data ) {	
			$("#configform input").prop("disabled", false);
			$("input[name='opmode']:checked").removeAttr("checked");
			$("input[name='opmode']").val([data.state]).checkboxradio("refresh");
			
			$("#remoteurl").val(data.remoteurl);
			$("#startupcolor").spectrum("set", data.hexcolor);
			$("#hostname").val(data.hostname);
			$("#delay_before_going_remotecontrolled").val(data.delay_before_going_remotecontrolled);
			
			$("#configform input[type='radio']").checkboxradio("refresh");
		});
	});
	
	$('#configform').on('submit', function(evt) {
		//convert the color value to something easy to digest at the controller side
		var newcolor = $("#startupcolor").spectrum('get').toHexString();
		var color = tinycolor(newcolor);
		$("input[name='startupcolor_r']").val(Math.round(color._r).toString());
		$("input[name='startupcolor_g']").val(Math.round(color._g).toString());
		$("input[name='startupcolor_b']").val(Math.round(color._b).toString());
		
		// transmit form
		$.get("/config.json?"+$('#configform').serialize(), function( data ) {
			debug("values submitted");
			$.mobile.back();
		});

		evt.preventDefault();
	});
	
	$('.quickButtons,#sOn,#sOff,#sRC,#sCC').on('click', function(evt, ui) {
		debug($(this).attr("href"));
		evt.preventDefault();
		
		$.get($(this).attr("href"), function( data ) {
			debug("done");
		});
	});
	
	$('#rebootbtn').on('click', function(evt, ui) {
		debug('Sending restart command...');
		$.get("/reset", function( data ) {
			debug(data);
			$.mobile.back();
		});
	});
});