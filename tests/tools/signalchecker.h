
#include <QObject>

class SignalCounter;

class SignalChecker {

    public:
        SignalChecker(QObject *obj);
        ~SignalChecker();
        void addSignalChecker(const char *signal);
        void check();
        void increaseSigCounter(const char *signal);
   private:
        QHash<QString, SignalCounter*> m_signalHasher;
        QObject *m_objToMonitor;
};

