CREATE TABLE Personas(
dni STRING,
nombre STRING NOT NULL
);

CREATE TABLE Insumos(
id INT PRIMARY KEY,
descripcion STRING NOT NULL,
)

CREATE TABLE ES(
id INT PRIMARY KEY,
insumo INT,
fecha LONG,
cantidad INT,
procedencia STRING,
responsable int,
recibido int,
);
