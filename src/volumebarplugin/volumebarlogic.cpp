/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "volumebarlogic.h"

#include <stdlib.h>
#include <string.h>
#include <QVariant>
#include <QString>

#define DEBUG
#define WARNING
#include "../debug.h"

#define DBUS_ERR_CHECK(err) \
    if (dbus_error_is_set (&err)) \
    { \
        SYS_WARNING (err.message); \
        dbus_error_free (&err); \
    }

static void
stepsUpdatedSignal (DBusConnection *conn,
                    DBusMessage    *message,
                    VolumeBarLogic *logic);

#define DEFAULT_ADDRESS "unix:path=/var/run/pulse/dbus-socket"

#define VOLUME_SV    "com.Nokia.MainVolume1"
#define VOLUME_PATH  "/com/nokia/mainvolume1"
#define VOLUME_IF    "com.Nokia.MainVolume1"


VolumeBarLogic::VolumeBarLogic () :
    QObject (),
    m_dbus_conn (0),
    m_eventloop (0),
    m_currentvolume (0),
    m_currentmax (0)
{
    DBusError dbus_err;
    char *pa_bus_address = getenv ("PULSE_DBUS_SERVER");

    if (pa_bus_address == NULL)
        pa_bus_address = DEFAULT_ADDRESS;

    dbus_error_init (&dbus_err);

    m_dbus_conn = dbus_connection_open (pa_bus_address, &dbus_err);

    DBUS_ERR_CHECK (dbus_err);

    if ((m_dbus_conn != NULL) && 
        (m_eventloop = new DBUSConnectionEventLoop) &&
        (m_eventloop->addConnection (m_dbus_conn)))
    {
        dbus_bus_add_match (m_dbus_conn, "type='signal',"
                            "interface='" VOLUME_IF "'", NULL);
        dbus_connection_add_filter (
            m_dbus_conn,
            (DBusHandleMessageFunction) stepsUpdatedSignal,
            (void *) this, NULL);
    }

    initValues ();
}

VolumeBarLogic::~VolumeBarLogic ()
{
    if (m_dbus_conn)
    {
        dbus_connection_unref (m_dbus_conn);
        m_dbus_conn = 0;

        if (m_eventloop)
        {
            delete m_eventloop;
            m_eventloop = 0;
        }
    }
}

void
VolumeBarLogic::initValues ()
{
    DBusMessage *msg;
    DBusMessage *reply;
    DBusError    error;

    dbus_error_init (&error);

    msg = dbus_message_new_method_call (VOLUME_SV, // "org.freedesktop.DBus",
                                        VOLUME_PATH, // "/org/freedesktop/DBus",
                                        "org.freedesktop.DBus.Properties",
                                        "GetAll");
    const char *volume_if = VOLUME_IF;
    dbus_message_append_args (msg,
                              DBUS_TYPE_STRING, &volume_if,
                              DBUS_TYPE_INVALID);

    reply =
        dbus_connection_send_with_reply_and_block (
                        m_dbus_conn, msg, -1, &error);

    DBUS_ERR_CHECK (error);

    dbus_message_unref (msg);
                            
    if (reply && (dbus_message_get_type (reply) ==
                  DBUS_MESSAGE_TYPE_METHOD_RETURN))
    {
        DBusMessageIter iter;        
        dbus_message_iter_init (reply, &iter);
        // Recurse into the array [array of dicts]
        while (dbus_message_iter_get_arg_type (&iter) != DBUS_TYPE_INVALID)
        {
            DBusMessageIter dict_entry;
            dbus_message_iter_recurse (&iter, &dict_entry);

            // Recurse into the dict [ dict_entry (string, variant(int)) ]
            while (dbus_message_iter_get_arg_type (&dict_entry) != DBUS_TYPE_INVALID)
            {
                DBusMessageIter in_dict; 
                // Recurse into the dict_entry [ string, variant(int) ]
                dbus_message_iter_recurse (&dict_entry, &in_dict);
                {
                  char *prop_name;
                  // Get the string value, "property name"
                  dbus_message_iter_get_basic (&in_dict, &prop_name);

                  dbus_message_iter_next (&in_dict);

                  DBusMessageIter variant;
                  // Recurese into the variant [ variant(int) ]
                  dbus_message_iter_recurse (&in_dict, &variant);

                  quint32 value;
                  // Get the variant value which is uint32
                  dbus_message_iter_get_basic (&variant, &value);

                  if (prop_name &&
                      strcmp (prop_name, "StepCount") == 0)
                    m_currentmax = value;
                  else if (prop_name &&
                           strcmp (prop_name, "CurrentStep") == 0)
                    m_currentvolume = value;

                  SYS_DEBUG ("%s: %d", prop_name, value);
                }

                dbus_message_iter_next (&dict_entry);
            }
            dbus_message_iter_next (&iter);
        }
    }

    if (reply)
        dbus_message_unref (reply);
}

#if 0
#define DBUS_ARG_TYPE(type) \
          switch (type) { \
            case DBUS_TYPE_INVALID: \
                SYS_DEBUG ("invalid"); \
                break; \
            case DBUS_TYPE_STRING: \
                SYS_DEBUG ("string"); \
                break; \
            case DBUS_TYPE_UINT32: \
                SYS_DEBUG ("uint32"); \
                break; \
            case DBUS_TYPE_ARRAY: \
                SYS_DEBUG ("array"); \
                break; \
            case DBUS_TYPE_STRUCT: \
                SYS_DEBUG ("stuct"); \
                break; \
            case DBUS_TYPE_DICT_ENTRY: \
                SYS_DEBUG ("dict_entry"); \
                break; \
            default: \
                SYS_DEBUG ("type_code %d", type); \
                break; \
          }
#endif

static void
stepsUpdatedSignal (DBusConnection *conn,
                    DBusMessage    *message,
                    VolumeBarLogic *logic)
{
    Q_UNUSED (conn);
    Q_UNUSED (message);
    Q_UNUSED (logic);
    SYS_DEBUG ("");

#if 0
    quint32 value = 0;
    quint32 maxvalue = 0;

    DBusMessageIter iter;        
    dbus_message_iter_init (reply, &iter);
    // Recurse into the array [array of dicts]
    while (dbus_message_iter_get_arg_type (&iter) != DBUS_TYPE_INVALID)
    {
        DBusMessageIter dict_entry;
        dbus_message_iter_recurse (&iter, &dict_entry);
    }
#endif

#if 0
    // Forward the data to the BusinessLogic 
    logic->stepsUpdated (value, maxvalue);
#endif
}


void
VolumeBarLogic::stepsUpdated (quint32 value, quint32 maxvalue)
{
    SYS_DEBUG ("val = %d [max = %d]", value, maxvalue);

    m_currentvolume = value;
    m_currentmax = maxvalue;

    emit volumeChanged (value, maxvalue);
}

void
VolumeBarLogic::setVolume (quint32 value)
{
    SYS_DEBUG ("value = %d", value);

//    m_if->setProperty ("CurrentStep", QVariant (value));
}

quint32
VolumeBarLogic::getVolume ()
{
    SYS_DEBUG ("volume = %d", m_currentvolume);

    return m_currentvolume;
}

quint32
VolumeBarLogic::getMaxVolume ()
{
    SYS_DEBUG ("max = %d", m_currentmax);

    return m_currentmax;
}

