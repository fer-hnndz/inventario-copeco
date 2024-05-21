CREATE TABLE Personas(
dni varchar(15) NOT NULL,
nombre text NOT NULL,
password text NOT NULL
PRIMARY KEY (dni)
);

CREATE TABLE Insumos(
id int NOT NULL,
descripcion text NOT NULL,
PRIMARY KEY(id)
);

CREATE TABLE ES(
id int NOT NULL,
insumo int,
fecha timestamp,
cantidad int,
procedencia text,
responsable varchar(15),
recibido varchar(15),
PRIMARY KEY(id),
FOREIGN KEY(responsable) REFERENCES Personas(dni),
FOREIGN KEY(recibido) REFERENCES Personas(dni)
);
