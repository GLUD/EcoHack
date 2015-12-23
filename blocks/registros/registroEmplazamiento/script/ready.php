// Asociar el widget de validación al formulario

/////////Se define el ancho de los campos de listas desplegables///////

$("#<?php echo $this->campoSeguro('id_tipo_emplazamiento');?>").width(575);

//////////////////**********Se definen los campos que requieren campos de select2**********////////////////

$("#<?php echo $this->campoSeguro('id_tipo_emplazamiento');?>").select2();
