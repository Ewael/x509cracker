#!/usr/bin/env python3

import sqlite3

def create_connection(db_file):
    """
    Try to connect to the given database
    """
    conn = None
    try:
        conn = sqlite3.connect(db_file)
    except:
        raise Exception("Can't connect to database")
    return conn

def insert_certificate(cert, id_db):
    """
    Insert certificate in the databse
    """

    database = "./data/cert_x509-" + str(id_db) + ".db"
    conn = create_connection(database)
    with conn:
        values = (cert[0], cert[1], cert[2], cert[3], hex(int(cert[4])), cert[5]);

        req = ''' INSERT INTO certificates(id, common_name, validity_start, validity_end, modulus, certificate)
                VALUES(?, ?, ?, ?, ?, ?) '''

        req = 'INSERT INTO certificates(id, common_name, validity_start,'
        req += 'validity_end, modulus, certificate)\n'
        req += 'VALUES(?, ?, ?, ?, ?, ?) '
        cur = conn.cursor()
        cur.execute(req, values)
        conn.commit()
    return cur

def select_modulus():
    """
    Select every modulus into database
    """
    database = "cert_x509.db"
    conn = create_connection(database)
    with conn:
        req = 'SELECT modulus FROM certificates'
        cur = conn.cursor()
        cur.execute(req)
        rows = cur.fetchall()
        print(rows)
    return rows
