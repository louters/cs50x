function submit_contact_form()
{
    $("#alertSuccess").removeClass("hide");
    $("#alertSuccess").addClass("show");

    setTimeout(gotohome, 1500);
}

function alert_closed()
{
    $("#alertSuccess").bind("closed", gotohome);
}

function gotohome()
{
    window.location.href = "index.html";
}