var newwindow;
var t_ext = new Array('t50', 't75', 't100', 't150', 't200');

function openWindow(url, title){
	newwindow = window.open(url, title, 'width=500, height=500, resizable=yes, status=yes, scrollbars=yes');
        if (window.focus) {newwindow.focus()} 
}

function changeWindow(oldt, manyWindows){
	var url = newwindow.location.href;
	var tselect = document.selForm.thresh;
	var newt = tselect.options[tselect.selectedIndex].value;
	var ind = url.indexOf(t_ext[oldt]);
	if(ind >= 0){	
		var newURL = url.substring(0, ind);
		var newURL = newURL + t_ext[newt] + ".html";
		//newwindow.location = newURL;
		if(manyWindows)
			openWindow(newURL, '');
		else
			openWindow(newURL, 'picwin');
	}	
}
