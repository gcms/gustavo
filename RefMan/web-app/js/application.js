var Ajax;
if (Ajax && (Ajax != null)) {
    Ajax.Responders.register({
        onCreate: function() {
            if ($('spinner') && Ajax.activeRequestCount > 0)
                Effect.Appear('spinner', {duration:0.5,queue:'end'});
        },
        onComplete: function() {
            if ($('spinner') && Ajax.activeRequestCount == 0)
                Effect.Fade('spinner', {duration:0.5,queue:'end'});
        }
    });
}


function show(a, id) {
    var el = document.getElementById(id);
    el.style.display = 'block';

    a.onclick = function() {
        hide(a, id)
    };
    a.innerHTML = 'Hide details';
}

function hide(a, id) {
    var el = document.getElementById(id);
    el.style.display = 'none';

    a.onclick = function() {
        show(a, id)
    };
    a.innerHTML = 'Show details';
}

