#include "searchtexpaths.h"


#if defined(Q_OS_WIN)
QString searchInWindowPath(const QString &filter = QString())
{
    QByteArray pEnv = qgetenv("PATH");
    const QLatin1Char pathSep(';');
    QStringList searchPaths;
    const QStringList rawPaths = QString::fromLocal8Bit(pEnv.constData()).split(pathSep, QString::SkipEmptyParts);
    searchPaths.reserve(rawPaths.size());
    foreach (const QString &rawPath, rawPaths) {
        QString cleanPath = QDir::cleanPath(rawPath);
        if (cleanPath.size() > 1 && cleanPath.endsWith(QLatin1Char('/')))
            cleanPath.truncate(cleanPath.size() - 1);
        searchPaths.push_back(cleanPath);
    }
    QStringList path_found=searchPaths.filter(filter);
    if  (!path_found.empty()) return path_found.takeLast();
    else return QString();
}

enum REG_SEARCH_TYPE
{
    USER,
    SYSTEM_32BIT,
    SYSTEM_64BIT
};

QStringList getRegSubkeys(const HKEY handle)
{
    QStringList keys;

    DWORD cSubKeys = 0;
    DWORD cMaxSubKeyLen = 0;
    LONG res = ::RegQueryInfoKeyW(handle, NULL, NULL, NULL, &cSubKeys, &cMaxSubKeyLen, NULL, NULL, NULL, NULL, NULL, NULL);

    if (res == ERROR_SUCCESS) {
        ++cMaxSubKeyLen; // For null character
        LPWSTR lpName = new WCHAR[cMaxSubKeyLen];
        DWORD cName;

        for (DWORD i = 0; i < cSubKeys; ++i) {
            cName = cMaxSubKeyLen;
            res = ::RegEnumKeyExW(handle, i, lpName, &cName, NULL, NULL, NULL, NULL);
            if (res == ERROR_SUCCESS)
                keys.push_back(QString::fromWCharArray(lpName));
        }

        delete[] lpName;
    }

    return keys;
}

QString getRegValue(const HKEY handle, const QString &name = QString())
{
    QString result;

    DWORD type = 0;
    DWORD cbData = 0;
    LPWSTR lpValueName = NULL;
    if (!name.isEmpty()) {
        lpValueName = new WCHAR[name.size() + 1];
        name.toWCharArray(lpValueName);
        lpValueName[name.size()] = 0;
    }

    // Discover the size of the value
    ::RegQueryValueExW(handle, lpValueName, NULL, &type, NULL, &cbData);
    DWORD cBuffer = (cbData / sizeof(WCHAR)) + 1;
    LPWSTR lpData = new WCHAR[cBuffer];
    LONG res = ::RegQueryValueExW(handle, lpValueName, NULL, &type, (LPBYTE)lpData, &cbData);
    if (lpValueName)
        delete[] lpValueName;

    if (res == ERROR_SUCCESS) {
        lpData[cBuffer - 1] = 0;
        result = QString::fromWCharArray(lpData);
    }
    delete[] lpData;

    return result;
}

QString miktexSearchReg(const REG_SEARCH_TYPE type)
{
    HKEY hkRoot;
    if (type == USER)
        hkRoot = HKEY_CURRENT_USER;
    else
        hkRoot = HKEY_LOCAL_MACHINE;

    REGSAM samDesired = KEY_READ;
    if (type == SYSTEM_32BIT)
        samDesired |= KEY_WOW64_32KEY;
    else if (type == SYSTEM_64BIT)
        samDesired |= KEY_WOW64_64KEY;

    QString path, miktex_root;
    LONG res = 0;
    HKEY hkMiktex;
    res = ::RegOpenKeyExW(hkRoot, L"SOFTWARE\\MiKTeX.org\\MiKTeX", 0, samDesired, &hkMiktex);

    if (res == ERROR_SUCCESS) {
        QStringList versions = getRegSubkeys(hkMiktex);
        versions.sort();


        bool found = false;
        if  (!versions.empty()) {
            const QString core = versions.takeLast() + "\\Core";
            LPWSTR lpSubkeyCore = new WCHAR[core.size() + 1];
            core.toWCharArray(lpSubkeyCore);
            lpSubkeyCore[core.size()] = 0;
            HKEY hkMiktexCore;
            res = ::RegOpenKeyExW(hkMiktex, lpSubkeyCore, 0, samDesired, &hkMiktexCore);
            delete[] lpSubkeyCore;
            if (res == ERROR_SUCCESS) {
                miktex_root= getRegValue(hkMiktexCore,"UserInstall");
                ::RegCloseKey(hkMiktexCore);
                path=miktex_root+"\\miktex\\bin\\x64";
                if (!path.isEmpty() && QDir(path).exists("latex.exe")) {
                    found = true;
                    path = QDir(path).absolutePath();
                }
                if (!found) {
                    path=miktex_root+"\\miktex\\bin";
                    if (!path.isEmpty() && QDir(path).exists("latex.exe")) {
                        found = true;
                        path = QDir(path).absolutePath();
                    }
                }
            }
        }
        if (!found)
            path = QString();

        ::RegCloseKey(hkMiktex);
    }

    return path;
}

QString findMiktexPath()
{

    QString path = miktexSearchReg(USER);
    if (!path.isEmpty())
        return path;

    path = miktexSearchReg(SYSTEM_32BIT);
    if (!path.isEmpty())
        return path;

    path = miktexSearchReg(SYSTEM_64BIT);
    if (!path.isEmpty())
        return path;

    path=searchInWindowPath("miktex/bin");
    if (!path.isEmpty())
        return path;

    return QString("C:/Program Files/MiKTeX 2.9/miktex/bin/x64");
}


QString nativeAsymptoteSearchReg(const REG_SEARCH_TYPE type)
{
    HKEY hkRoot;
    if (type == USER)
        hkRoot = HKEY_CURRENT_USER;
    else
        hkRoot = HKEY_LOCAL_MACHINE;

    REGSAM samDesired = KEY_READ;
    if (type == SYSTEM_32BIT)
        samDesired |= KEY_WOW64_32KEY;
    else if (type == SYSTEM_64BIT)
        samDesired |= KEY_WOW64_64KEY;

    QString path;
    LONG res = 0;
    HKEY hkAsymptote;
    res = ::RegOpenKeyExW(hkRoot, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Asymptote", 0, samDesired, &hkAsymptote);

    if (res == ERROR_SUCCESS) {
        bool found = false;
        path= getRegValue(hkAsymptote,"Path");
        if (!path.isEmpty() && QDir(path).exists("asy.exe"))
        {
            found = true;
            path = QDir(path).absolutePath();
        }
        if (!found)  path = QString();
        ::RegCloseKey(hkAsymptote);
    }

    return path;
}
QString findNativeAsymptotePath()
{
    QString path = nativeAsymptoteSearchReg(USER);
    if (!path.isEmpty())
        return path;

    path = nativeAsymptoteSearchReg(SYSTEM_32BIT);
    if (!path.isEmpty())
        return path;

    path = nativeAsymptoteSearchReg(SYSTEM_64BIT);
    if (!path.isEmpty())
        return path;

    return QString();
}

QString findAsymptotePath()
{
    QString path=findNativeAsymptotePath();
    if (!path.isEmpty())
        return path;

    path="C:/Program Files (x86)/Asymptote";
    if (QDir(path).exists("asy.exe"))
        return path;

    path="C:/Program Files/Asymptote";
    if (QDir(path).exists("asy.exe"))
        return path;

    path=searchInWindowPath("texlive");
    if (!path.isEmpty() && QDir(path).exists("asy.exe"))
        return path;

    path=searchInWindowPath("miktex/bin");
    if (!path.isEmpty() && QDir(path).exists("asy.exe"))
        return path;


    return QString("");
}

QString acrobatExeSearchReg(const REG_SEARCH_TYPE type)
{
    HKEY hkRoot=HKEY_CLASSES_ROOT;


    REGSAM samDesired = KEY_READ;
    if (type == SYSTEM_32BIT)
        samDesired |= KEY_WOW64_32KEY;
    else if (type == SYSTEM_64BIT)
        samDesired |= KEY_WOW64_64KEY;

    QString path;
    LONG res = 0;
    HKEY hkAcrobat;
    res = ::RegOpenKeyExW(hkRoot, L"SOFTWARE\\Adobe\\Acrobat\\Exe", 0, samDesired, &hkAcrobat);

    if (res == ERROR_SUCCESS) {
        bool found = false;
        path= getRegValue(hkAcrobat);
        if (!path.isEmpty() )
        {
            path=path.remove("\"");
            path=QFileInfo(path).absoluteFilePath();
            if (QFileInfo(path).exists()) found=true;

        }
        if (!found)  path = QString();
        ::RegCloseKey(hkAcrobat);
    }
    return path;
}

QString findAcrobatExePath()
{
    QString path = acrobatExeSearchReg(SYSTEM_32BIT);
    if (!path.isEmpty())
        return path;

    path = acrobatExeSearchReg(SYSTEM_64BIT);
    if (!path.isEmpty())
        return path;


    return QString("C:/Program Files (x86)/Adobe/Acrobat Reader DC/Reader/AcroRd32.exe");
}

QString ghostscriptSearchReg(const REG_SEARCH_TYPE type)
{
    HKEY hkRoot;
    if (type == USER)
        hkRoot = HKEY_CURRENT_USER;
    else
        hkRoot = HKEY_LOCAL_MACHINE;

    REGSAM samDesired = KEY_READ;
    if (type == SYSTEM_32BIT)
        samDesired |= KEY_WOW64_32KEY;
    else if (type == SYSTEM_64BIT)
        samDesired |= KEY_WOW64_64KEY;

    QString path, gs_root;
    LONG res = 0;
    HKEY hkGS;
    res = ::RegOpenKeyExW(hkRoot, L"SOFTWARE\\GPL Ghostscript", 0, samDesired, &hkGS);

    if (res == ERROR_SUCCESS) {
        QStringList versions = getRegSubkeys(hkGS);
        versions.sort();


        bool found = false;
        if  (!versions.empty()) {
            const QString core = versions.takeLast() ;
            LPWSTR lpSubkeyCore = new WCHAR[core.size() + 1];
            core.toWCharArray(lpSubkeyCore);
            lpSubkeyCore[core.size()] = 0;
            HKEY hkGSCore;
            res = ::RegOpenKeyExW(hkGS, lpSubkeyCore, 0, samDesired, &hkGSCore);
            delete[] lpSubkeyCore;
            if (res == ERROR_SUCCESS) {
                gs_root= getRegValue(hkGSCore,"GS_LIB");
                ::RegCloseKey(hkGSCore);
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                QStringList paths = gs_root.split(";", QString::SkipEmptyParts);
#else
                QStringList paths = gs_root.split(";", Qt::SkipEmptyParts);
#endif
                if (!paths.empty())
                {
                    QString binpath=paths.takeFirst();
                    if (!binpath.isEmpty() && QDir(binpath).exists("gswin64c.exe")) {
                        found = true;
                        path = QDir(binpath).absolutePath()+"/gswin64c.exe";
                    }
                    if (!found) {
                        if (!binpath.isEmpty() && QDir(binpath).exists("gswin32c.exe")) {
                            found = true;
                            path = QDir(binpath).absolutePath()+"/gswin32c.exe";
                        }
                    }
                }
            }
        }
        if (!found)
            path = QString();

        ::RegCloseKey(hkGS);
    }

    return path;
}

QString findGhostscriptPath()
{

    QString path = ghostscriptSearchReg(USER);
    if (!path.isEmpty())
        return path;

    path = ghostscriptSearchReg(SYSTEM_32BIT);
    if (!path.isEmpty())
        return path;

    path = ghostscriptSearchReg(SYSTEM_64BIT);
    if (!path.isEmpty())
        return path;

    return QString("gswin64c.exe");
}

QString RSearchReg(const REG_SEARCH_TYPE type)
{
    HKEY hkRoot;
    if (type == USER)
        hkRoot = HKEY_CURRENT_USER;
    else
        hkRoot = HKEY_LOCAL_MACHINE;

    REGSAM samDesired = KEY_READ;
    if (type == SYSTEM_32BIT)
        samDesired |= KEY_WOW64_32KEY;
    else if (type == SYSTEM_64BIT)
        samDesired |= KEY_WOW64_64KEY;

    QString path,rcore_root;
    LONG res = 0;
    HKEY hkR;
    res = ::RegOpenKeyExW(hkRoot, L"SOFTWARE\\R-core\\R", 0, samDesired, &hkR);

    if (res == ERROR_SUCCESS) {
        QStringList versions = getRegSubkeys(hkR);
        versions.sort();


        bool found = false;
        if  (!versions.empty()) {
            const QString core = versions.takeLast() ;
            LPWSTR lpSubkeyCore = new WCHAR[core.size() + 1];
            core.toWCharArray(lpSubkeyCore);
            lpSubkeyCore[core.size()] = 0;
            HKEY hkRCore;
            res = ::RegOpenKeyExW(hkR, lpSubkeyCore, 0, samDesired, &hkRCore);
            delete[] lpSubkeyCore;
            if (res == ERROR_SUCCESS) {
                rcore_root= getRegValue(hkRCore,"InstallPath");
                ::RegCloseKey(hkRCore);
                path=rcore_root+"/bin/x64/";
                if (!path.isEmpty() && QDir(path).exists("R.exe")) {
                    found = true;
                    path = QDir(path).absolutePath()+"/R.exe";
                }
                if (!found) {
                    path=rcore_root+"/bin/i386/";
                    if (!path.isEmpty() && QDir(path).exists("R.exe")) {
                        found = true;
                        path = QDir(path).absolutePath()+"/R.exe";
                    }
                }
            }
        }

        if (!found)
            path = QString();

        ::RegCloseKey(hkR);
    }

    return path;
}

QString findRPath()
{

    QString path = RSearchReg(USER);
    if (!path.isEmpty())
        return path;

    path = RSearchReg(SYSTEM_32BIT);
    if (!path.isEmpty())
        return path;

    path = RSearchReg(SYSTEM_64BIT);
    if (!path.isEmpty())
        return path;

    return QString("R.exe");
}
#endif // Q_OS_WIN
SearchTexPaths::SearchTexPaths()
{

}
