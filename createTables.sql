-- Existente Tabellen löschen
    DROP TABLE IF EXISTS room;
    DROP TABLE IF EXISTS air_quality_measure;
-- ////////////////////////////////////////////////////

-- Tabellen erstellen
    CREATE TABLE room (
        id CHAR(36) DEFAULT (UUID()) PRIMARY KEY,
        name VARCHAR(255) NOT NULL UNIQUE,
        creation_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );

    CREATE TABLE air_quality_measure (
        id CHAR(36) DEFAULT (UUID()) PRIMARY KEY,
        room CHAR(36) NOT NULL REFERENCES room(id),
        air_quality INTEGER NOT NULL,
        creation_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
-- ////////////////////////////////////////////////////

-- Tabellen befüllen
    INSERT INTO room (name)
    VALUES 
    ('Eduzone'),
    ('Foyer'),
    ('I-0.11'),
    ('I-0.30'),
    ('I-0.31');
-- ////////////////////////////////////////////////////