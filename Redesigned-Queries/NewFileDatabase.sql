BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Fields" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Description"	TEXT,
	PRIMARY KEY("Id")
);
CREATE TABLE IF NOT EXISTS "Exercise_Types" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL,
	PRIMARY KEY("Id")
);
CREATE TABLE IF NOT EXISTS "Sections" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Field"	TEXT NOT NULL,
	"Chapter"	TEXT NOT NULL,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("Chapter") REFERENCES "Chapters"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	PRIMARY KEY("Id")
);
CREATE TABLE IF NOT EXISTS "Sections_Exercises" (
	"Exercise_Id"	TEXT NOT NULL,
	"Exercise_Name"	TEXT NOT NULL,
	"Section_Id"	TEXT NOT NULL,
	FOREIGN KEY("Exercise_Id") REFERENCES "Exercise_Types"("Id") ON UPDATE CASCADE,
	FOREIGN KEY("Section_Id") REFERENCES "Sections"("Id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "BackUp" (
	"Table_Id"	TEXT,
	"Id"	TEXT,
	"Name"	TEXT
);
CREATE TABLE IF NOT EXISTS "Chapters" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Field"	TEXT NOT NULL,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	PRIMARY KEY("Id")
);
CREATE TABLE IF NOT EXISTS "FileTypes" (
	"Id"	TEXT NOT NULL,
	"FileType"	TEXT NOT NULL,
	"FolderName"	TEXT NOT NULL,
	"Solvable"	INTEGER,
	"BelongsTo"	TEXT,
	PRIMARY KEY("Id")
);
CREATE TABLE IF NOT EXISTS "Bibliography" (
	"Citation_Key"	TEXT,
	"Document_Type"	TEXT,
	"title"	TEXT,
	"author"	TEXT,
	"editor"	TEXT,
	"publisher"	TEXT,
	"year"	INTEGER,
	"month"	INTEGER,
	"isbn"	INTEGER,
	"issn"	INTEGER,
	"pages"	INTEGER,
	"series"	TEXT,
	"volume"	INTEGER,
	"journal"	TEXT,
	"institution"	TEXT,
	"issue"	INTEGER,
	"address"	TEXT,
	"doi"	INTEGER,
	"url"	TEXT,
	"edition"	TEXT,
	"UseBibliography"	INTEGER
);
CREATE TABLE IF NOT EXISTS "Sections_per_File" (
	"File_Id"	TEXT,
	"Section_Id"	TEXT
);
CREATE TABLE IF NOT EXISTS "Chapters_per_File" (
	"File_Id"	TEXT,
	"Chapter_Id"	TEXT
);
CREATE TABLE IF NOT EXISTS "Database_Files" (
	"Id"	TEXT NOT NULL,
	"FileType"	TEXT,
	"Field"	INTEGER,
	"Difficulty"	INTEGER,
	"Path"	TEXT NOT NULL,
	"Date"	TEXT,
	"Solved"	TEXT,
	"Bibliography"	TEXT,
	"FileContent"	TEXT,
	"Preamble"	TEXT,
	"BuildCommand"	TEXT,
	"FileDescription"	TEXT,
	"MultiSection"	INTEGER,
	"Class"	TEXT,
	"Year"	INTEGER,
	"Semester"	INTEGER,
	PRIMARY KEY("Id"),
	FOREIGN KEY("FileType") REFERENCES "FileTypes"("Id") ON UPDATE CASCADE ON DELETE SET NULL,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE SET NULL
);
CREATE TABLE IF NOT EXISTS "ExerciseTypes_per_File" (
	"File_Id"	TEXT,
	"ExerciseType_Id"	TEXT
);
INSERT INTO "Fields" VALUES ('Algebra','Άλγεβρα',NULL);
INSERT INTO "Fields" VALUES ('Geometria','Γεωμετρία',NULL);
INSERT INTO "Fields" VALUES ('Analysh','Ανάλυση',NULL);
INSERT INTO "Fields" VALUES ('Pithanothtes','Πιθανότητες',NULL);
INSERT INTO "Fields" VALUES ('Statistikh','Στατιστική',NULL);
INSERT INTO "Fields" VALUES ('-','-',NULL);
INSERT INTO "Exercise_Types" VALUES ('1oThewr','Πρώτο θεώρημα διαμέσων');
INSERT INTO "Exercise_Types" VALUES ('2oThewr','Δεύτερο θεώρημα διαμέσων');
INSERT INTO "Exercise_Types" VALUES ('ARrAn2Riz','Άρρητες ανισώσεις με δύο ρίζες');
INSERT INTO "Exercise_Types" VALUES ('AfairAri','Αφαίρεση αριθμών');
INSERT INTO "Exercise_Types" VALUES ('AkAnadTyp','Αναδρομικός τύπος ακολουθίας');
INSERT INTO "Exercise_Types" VALUES ('AkGenTyp','Γενικός τύπος ακολουθίας');
INSERT INTO "Exercise_Types" VALUES ('AkrKlDiast','Ακρότατα με τη χρήση μονοτονίας σε κλειστό διάστημα');
INSERT INTO "Exercise_Types" VALUES ('AkrotAnis','Εύρεση ακροτάτων με βασικές ανισότητες');
INSERT INTO "Exercise_Types" VALUES ('AkrotPar','Εύρεση ακροτάτων με τη χρήση παραγώγου');
INSERT INTO "Exercise_Types" VALUES ('AktDiam','Ακτίνα - Διάμετρος - Κέντρο κύκλου');
INSERT INTO "Exercise_Types" VALUES ('AlgParRiz','Πεδίο ορισμού άρρητης αλγεβρικής παράστασης');
INSERT INTO "Exercise_Types" VALUES ('AllagBash','Αλλαγή βάσης λογαρίθμου');
INSERT INTO "Exercise_Types" VALUES ('An2Trig','Ανισότητα πλευρών - γωνιών σε δύο τρίγωνα');
INSERT INTO "Exercise_Types" VALUES ('AnHmx','Η ανισότητα |ημx|<=x');
INSERT INTO "Exercise_Types" VALUES ('AnPlGwn','Ανισότητα πλευρών και γωνιών τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('AnTypGenT','Μετατροπή αναδρομικού τύπου σε γενικό και αντίστροφα');
INSERT INTO "Exercise_Types" VALUES ('AnaPerVenn','Μετατροπή συνόλου ανάμεσα σε αναγραφή - περιγραφή και διάγραμμα Venn');
INSERT INTO "Exercise_Types" VALUES ('Anag1oTet','Αναγωγή στο 1ο τεταρτημόριο');
INSERT INTO "Exercise_Types" VALUES ('AnagTyp','Αναγωγικοί τύποι');
INSERT INTO "Exercise_Types" VALUES ('Anagrafh','Αναγραφή συνόλου');
INSERT INTO "Exercise_Types" VALUES ('AnakIdiot','Ανακλαστική ιδιότητα');
INSERT INTO "Exercise_Types" VALUES ('AnalPrwt','Ανάλυση αριθμού σε γινόμενο πρώτων παραγόντων');
INSERT INTO "Exercise_Types" VALUES ('AnapKybou','Ανάπτυγμα κύβου');
INSERT INTO "Exercise_Types" VALUES ('AnapTetr','Ανάπτυγμα τετραγώνου');
INSERT INTO "Exercise_Types" VALUES ('AnisAnt','Επίλυση ανίσωσης με αντικατάσταση');
INSERT INTO "Exercise_Types" VALUES ('AnisApT','Ανισώσεις με απόλυτες τιμές');
INSERT INTO "Exercise_Types" VALUES ('AnisGin','Ανισώσεις γινόμενο - Πρόσημο Πολυωνύμου');
INSERT INTO "Exercise_Types" VALUES ('AnisHmSyn','Ανισοτικές σχέσεις ημιτόνου και συνημιτόνου');
INSERT INTO "Exercise_Types" VALUES ('AntPythT','Αντίστροφο Πυθαγορείου θεωρήματος');
INSERT INTO "Exercise_Types" VALUES ('Antipar','Αντιπαράδειγμα');
INSERT INTO "Exercise_Types" VALUES ('Antistoix','Αντιστοίχιση');
INSERT INTO "Exercise_Types" VALUES ('AntistrAr','Αντίστροφοι αριθμοί');
INSERT INTO "Exercise_Types" VALUES ('AntistrThal','Αντίστροφο θεωρήματος Θαλή');
INSERT INTO "Exercise_Types" VALUES ('Antistyx','Οι εξισώσεις f^(-1)(x)=x - f^(-1)(x)=f(x)');
INSERT INTO "Exercise_Types" VALUES ('AntithAr','Αντίθετοι αριθμοί');
INSERT INTO "Exercise_Types" VALUES ('AorAdynAn','Αόριστες και αδύνατες ανισώσεις 1ου βαθμού');
INSERT INTO "Exercise_Types" VALUES ('AorAdynEx','Αόριστες και αδύνατες εξισώσεις 1ου βαθμού');
INSERT INTO "Exercise_Types" VALUES ('Ap1p1Mon','Απόδειξη συνάρτησης 1 - 1 με χρήση μονοτονίας');
INSERT INTO "Exercise_Types" VALUES ('Ap1p1Or','Απόδειξη συνάρτησης 1 - 1 με χρήση ορισμού');
INSERT INTO "Exercise_Types" VALUES ('ApAkrAnis','Απόδειξη ακρότατου με τη βοήθεια ανισότητας');
INSERT INTO "Exercise_Types" VALUES ('ApEuth','Απόσταση μεταξύ παράλληλων ευθειών');
INSERT INTO "Exercise_Types" VALUES ('ApShmEpip','Απόσταση σημείου επιπέδου - παράλληλων επιπέδων');
INSERT INTO "Exercise_Types" VALUES ('ApTimAnis','Υπολογισμός παράστασης με απόλυτα με δοσμένη ανισότητα');
INSERT INTO "Exercise_Types" VALUES ('ApTimId','Ιδιότητες απόλυτων τιμών');
INSERT INTO "Exercise_Types" VALUES ('ApTimPer','Υπολογισμός παράστασης με απόλυτα με περιπτώσεις');
INSERT INTO "Exercise_Types" VALUES ('ApagAtopo','Απαγωγή σε άτοπο');
INSERT INTO "Exercise_Types" VALUES ('AplTrExis','Απλές τριγωνομετρικές εξισώσεις');
INSERT INTO "Exercise_Types" VALUES ('AplopKl','Απλοποίηση κλάσματος - Ανάγωγα κλάσματα');
INSERT INTO "Exercise_Types" VALUES ('AplopRht','Απλοποίηση ρητής παράστασης');
INSERT INTO "Exercise_Types" VALUES ('AplopRiz','Απλοποίηση ρίζας');
INSERT INTO "Exercise_Types" VALUES ('ApoTheorias','Αποδείξεις θεωρίας');
INSERT INTO "Exercise_Types" VALUES ('ApodAnis','Απόδειξη ανισότητας');
INSERT INTO "Exercise_Types" VALUES ('ApodEfapt','Συνθήκες για να εφάπτεται μια ευθεία');
INSERT INTO "Exercise_Types" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας');
INSERT INTO "Exercise_Types" VALUES ('ApoklDiaz','Αποκλειστική διάζευξη');
INSERT INTO "Exercise_Types" VALUES ('AposAnisD','Μετατροπή μεταξύ απόστασης αριθμών - ανισότητας με απόλυτα και διαστήματα');
INSERT INTO "Exercise_Types" VALUES ('AposAri','Απόσταση αριθμών');
INSERT INTO "Exercise_Types" VALUES ('ApostShm','Απόσταση σημείων');
INSERT INTO "Exercise_Types" VALUES ('ApostShmE','Απόσταση σημείου από ευθεία');
INSERT INTO "Exercise_Types" VALUES ('AposynSyn','Αποσύνθεση συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('Apotetr','Τύποι αποτετραγωνισμού γωνίας');
INSERT INTO "Exercise_Types" VALUES ('ArPrOrAth','Προσδιορισμός όρων α.π. από άθροισμα');
INSERT INTO "Exercise_Types" VALUES ('ArithMes','Αριθμητικός μέσος - Διαδοχικοί όροι αριθμητικής προόδου');
INSERT INTO "Exercise_Types" VALUES ('ArithPar','Αριθμητική παρεμβολή');
INSERT INTO "Exercise_Types" VALUES ('Arnhsh','Άρνηση');
INSERT INTO "Exercise_Types" VALUES ('ArrAn1Riz','Άρρητες ανισώσεις με μια ρίζα');
INSERT INTO "Exercise_Types" VALUES ('ArrEx1Riz','Άρρητες εξισώσεις με μια ρίζα');
INSERT INTO "Exercise_Types" VALUES ('ArrEx2Riz','Άρρητες εξισώσεις με δύο ρίζες');
INSERT INTO "Exercise_Types" VALUES ('ArtioiPer','Άρτιοι - Περιττοί αριθμοί');
INSERT INTO "Exercise_Types" VALUES ('ArtiosEkth','Πρόσημο δύναμης με άρτιο εκθέτη');
INSERT INTO "Exercise_Types" VALUES ('ArxBasSyn','Αρχική βασικών συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('ArxGinPil','Αρχική με κανόνα γινομένου ή πηλίκου');
INSERT INTO "Exercise_Types" VALUES ('ArxOria','Αρχική και όρια');
INSERT INTO "Exercise_Types" VALUES ('ArxPollTyp','Αρχική συνάρτησης πολλαπλού τύπου - απόλυτης τιμής');
INSERT INTO "Exercise_Types" VALUES ('ArxSynth','Αρχική σύνθετων συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('ArxThewr','Αρχική συνάρτηση και θεωρήματα');
INSERT INTO "Exercise_Types" VALUES ('AsymOria','Ασύμπτωτες και όρια');
INSERT INTO "Exercise_Types" VALUES ('AsymptYp','Ασύμπτωτες υπερβολής');
INSERT INTO "Exercise_Types" VALUES ('AthArPr','Άθροισμα ν πρώτων όρων αριθμητικής προόδου');
INSERT INTO "Exercise_Types" VALUES ('AthDiafK','Άθροισμα - Διαφορά κύβων');
INSERT INTO "Exercise_Types" VALUES ('AthGwn','Άθροισμα γωνιών');
INSERT INTO "Exercise_Types" VALUES ('AthGwnPol','Άθροισμα γωνιών ν-γωνου');
INSERT INTO "Exercise_Types" VALUES ('AthGwnTr','Αθροισμα γωνιών τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('AthrGeoPr','Άθροισμα ν πρώτων όρων γεωμετρικής προόδου');
INSERT INTO "Exercise_Types" VALUES ('AxiaPsif','Αξία ψηφίου');
INSERT INTO "Exercise_Types" VALUES ('AxiomXwr','Αξιώματα χώρου');
INSERT INTO "Exercise_Types" VALUES ('BSynVen','Βασικό σύνολο - Διάγραμμα Venn');
INSERT INTO "Exercise_Types" VALUES ('BarTrig','Βαρύκεντρο τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('Baryken','Βαρύκεντρο - ορθόκεντρο τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('BasEnnDian','Βασικές έννοιες διανυσμάτων');
INSERT INTO "Exercise_Types" VALUES ('BasEnnSt','Βασικές έννοιες στατιστικής');
INSERT INTO "Exercise_Types" VALUES ('BasGrPar','Γραφικές παραστάσεις βασικών συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('BasTaut','Βασικές πολυωνυμικές ταυτότητες');
INSERT INTO "Exercise_Types" VALUES ('BathMon','Βαθμός μονωνύμου');
INSERT INTO "Exercise_Types" VALUES ('BathmPol','Βαθμός πολυωνύμου');
INSERT INTO "Exercise_Types" VALUES ('BohthSyn','Βοηθητική συνάρτηση');
INSERT INTO "Exercise_Types" VALUES ('BolzLim','Θ. Bolzano με χρήση ορίων');
INSERT INTO "Exercise_Types" VALUES ('DekKlasm','Μετατροπή μεταξύ δεκαδικού αριθμού - κλάσματος');
INSERT INTO "Exercise_Types" VALUES ('DekadAr','Η έννοια του δεκαδικού αριθμού');
INSERT INTO "Exercise_Types" VALUES ('DeutStoix','Δευτερεύοντα στοιχεία τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('DiLathous','Διόρθωση λάθους');
INSERT INTO "Exercise_Types" VALUES ('DiadGwn','Διαδοχικές γωνίες');
INSERT INTO "Exercise_Types" VALUES ('DiadRolle','Διαδοχικές εφαρμογές του Θ. Rolle');
INSERT INTO "Exercise_Types" VALUES ('DiadSygkr','Διαδοχικές συγκρίσεις τριγώνων');
INSERT INTO "Exercise_Types" VALUES ('DiafTetr','Διαφορά τετραγώνων');
INSERT INTO "Exercise_Types" VALUES ('Diagramma','Διάγραμμα - Πολύγωνο συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('DiairAkKl','Διαίρεση ανάμεσα σε ακέραιο και κλάσμα');
INSERT INTO "Exercise_Types" VALUES ('DiairArith','Διαίρεση - Ευκλείδεια διαίρεση αριθμών');
INSERT INTO "Exercise_Types" VALUES ('DiairKl','Διαίρεση κλασμάτων');
INSERT INTO "Exercise_Types" VALUES ('DiairPol','Διαίρεση πολυωνύμων - Αλγόριθμος');
INSERT INTO "Exercise_Types" VALUES ('Diairetes','Διαιρέτες ακέραιου αριθμού');
INSERT INTO "Exercise_Types" VALUES ('Diakym','Διακύμανση');
INSERT INTO "Exercise_Types" VALUES ('Diamesos','Διάμεσος από παρατηρήσεις');
INSERT INTO "Exercise_Types" VALUES ('DianAktMes','Διανυσματική ακτίνα μέσου τμήματος');
INSERT INTO "Exercise_Types" VALUES ('DianIsot','Διανυσματικές ισότητες - Απόδειξη ιδιοτήτων');
INSERT INTO "Exercise_Types" VALUES ('DianThesis','Διάνυσμα θέσης - Διαφορά διανυσματικών ακτίνων');
INSERT INTO "Exercise_Types" VALUES ('DiaspKlasm','Διάσπαση κλάσματος σε άθροισμα');
INSERT INTO "Exercise_Types" VALUES ('Diasthmata','Διαστήματα');
INSERT INTO "Exercise_Types" VALUES ('DiaxXwr','Διαχωρισμός χωρίου');
INSERT INTO "Exercise_Types" VALUES ('Diazeuxh','Διάζευξη');
INSERT INTO "Exercise_Types" VALUES ('DiplAnis','Διπλές ανισότητες - ανισώσεις');
INSERT INTO "Exercise_Types" VALUES ('DiplAr','Τύπος διπλασιασμού του Αρχιμήδη');
INSERT INTO "Exercise_Types" VALUES ('DixotGwn','Διχοτόμος γωνίας');
INSERT INTO "Exercise_Types" VALUES ('Dixotomos','Διχοτόμος γωνίας');
INSERT INTO "Exercise_Types" VALUES ('DynArnBas','Δυνάμεις με αρνητική βάση - Αντίθετες βάσεις');
INSERT INTO "Exercise_Types" VALUES ('DynRhtEk','Δυνάμεις με ρητό εκθέτη');
INSERT INTO "Exercise_Types" VALUES ('Dynam10','Δυνάμεις του 10 - Τυποποιημένη μορφή');
INSERT INTO "Exercise_Types" VALUES ('DynamShm','Δύναμη σημείου ως προς κύκλο');
INSERT INTO "Exercise_Types" VALUES ('EKPAr','Ε.Κ.Π. αριθμών');
INSERT INTO "Exercise_Types" VALUES ('EKPMon','Ε.Κ.Π. - Μ.Κ.Δ. μονωνύμων');
INSERT INTO "Exercise_Types" VALUES ('EPKPol','Ε.Κ.Π. - Μ.Κ.Δ. πολυωνύμων');
INSERT INTO "Exercise_Types" VALUES ('EfExShm','Εφαπτομένη που διέρχεται από γνωστό σημείο');
INSERT INTO "Exercise_Types" VALUES ('EfGnKlish','Εφαπτομένη με γνωστή κλίση');
INSERT INTO "Exercise_Types" VALUES ('EfGnSEp','Εφαπτομένη με γνωστό σημείο επαφής');
INSERT INTO "Exercise_Types" VALUES ('EfParOTr','Εφαρμογές παραλληλογράμμων στα ορθογώνια τρ.');
INSERT INTO "Exercise_Types" VALUES ('EfRolle','Εφαρμογή Θ. Rolle');
INSERT INTO "Exercise_Types" VALUES ('EfThMET','Εφαρμογή θεωρήματος μέγιστης - ελάχιστης τιμής');
INSERT INTO "Exercise_Types" VALUES ('EfThMT','Εφαρομγή Θ.Μ.Τ.');
INSERT INTO "Exercise_Types" VALUES ('EfThalTr','Εφαρμογή του θ. Θαλή στα τρίγωνα');
INSERT INTO "Exercise_Types" VALUES ('EfapAnis','Εφαπτομένες και ανισότητες');
INSERT INTO "Exercise_Types" VALUES ('EfapOria','Εφαπτομένες και όρια');
INSERT INTO "Exercise_Types" VALUES ('EfapTmhm','Εφαπτόμενα τμήματα');
INSERT INTO "Exercise_Types" VALUES ('EfexhsGwn','Εφεξής γωνίες');
INSERT INTO "Exercise_Types" VALUES ('Egg81012','Εγγραφή κανονικού οκταγώνου - δεκαγώνου - δωδεκαγώνου');
INSERT INTO "Exercise_Types" VALUES ('EggBasP','Εγγραφή βασικών κανονικών πολυγώνων σε κύκλο');
INSERT INTO "Exercise_Types" VALUES ('EggKykl','Εγγεγραμμένος κύκλος');
INSERT INTO "Exercise_Types" VALUES ('EggTetr','Εγγεγραμμένο - εγγράψιμο τετράπλευρο');
INSERT INTO "Exercise_Types" VALUES ('EidMetD','Είδος μεταβλητής - Αντιπροσωπευτικό δείγμα');
INSERT INTO "Exercise_Types" VALUES ('EidhGwn','Είδη γωνιών');
INSERT INTO "Exercise_Types" VALUES ('EidhRizwn','Είδη και πλήθος ριζών τριωνύμου 2ου β.');
INSERT INTO "Exercise_Types" VALUES ('EidhTrig','Είδη τριγώνων (ως προς πλευρές και γωνίες)');
INSERT INTO "Exercise_Types" VALUES ('EidosGwn','Είδος γωνίας τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('EkfParKl','Έκφραση παράστασης σε κλάσμα');
INSERT INTO "Exercise_Types" VALUES ('Ekkentr','Εκκεντρότητα');
INSERT INTO "Exercise_Types" VALUES ('EkthAnis','Εκθετικές ανισώσεις');
INSERT INTO "Exercise_Types" VALUES ('EkthExis','Εκθετικές εξισώσεις');
INSERT INTO "Exercise_Types" VALUES ('EkthSyst','Εκθετικά συστήματα');
INSERT INTO "Exercise_Types" VALUES ('EllPinakas','Συμπλήρωση ελλειπή πίνακα');
INSERT INTO "Exercise_Types" VALUES ('EmbAntist','Εμβαδόν και αντίστροφή συνάρτηση');
INSERT INTO "Exercise_Types" VALUES ('EmbCfCg','Εμβαδόν μεταξύ δύο γραφικών παραστάσεων');
INSERT INTO "Exercise_Types" VALUES ('EmbCfEf','Εμβαδόν μεταξύ γρ. παράστασης και εφαπτομένης');
INSERT INTO "Exercise_Types" VALUES ('EmbCfxx','Εμβαδόν μεταξύ γρ. παράστασης και άξονα xx');
INSERT INTO "Exercise_Types" VALUES ('EmbDiam','Εμβαδόν και διάμεσος τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('EmbKykTm','Εμβαδόν κυκλικού τμήματος');
INSERT INTO "Exercise_Types" VALUES ('EmbKykTom','Εμβαδόν κυκλικού τομέα');
INSERT INTO "Exercise_Types" VALUES ('EmbKykl','Εμβαδόν κύκλου');
INSERT INTO "Exercise_Types" VALUES ('EmbMeikt','Εμβαδόν μεικτόγραμμου σχήματος');
INSERT INTO "Exercise_Types" VALUES ('EmbMhn','Εμβαδόν μηνίσκου');
INSERT INTO "Exercise_Types" VALUES ('EmbOrth','Εμβαδόν ορθογωνίου');
INSERT INTO "Exercise_Types" VALUES ('EmbPar','Εμβαδόν παραλληλογράμμου');
INSERT INTO "Exercise_Types" VALUES ('EmbPolEp','Εμβαδόν πολυγωνικής επιφάνειας');
INSERT INTO "Exercise_Types" VALUES ('EmbRomb','Εμβαδόν ρόμβου - τετραπλ. με κάθετες διαγ.');
INSERT INTO "Exercise_Types" VALUES ('EmbTetr','Εμβαδόν τετραγώνου');
INSERT INTO "Exercise_Types" VALUES ('EmbTrGP','Εμβαδόν μεταξύ τριών γραφικών παραστάσεων');
INSERT INTO "Exercise_Types" VALUES ('EmbTrap','Εμβαδόν τραπεζίου');
INSERT INTO "Exercise_Types" VALUES ('EmbTrig','Εμβαδόν τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('EnSynol','Έννοια συνόλου - Ανήκει - Δεν ανήκει');
INSERT INTO "Exercise_Types" VALUES ('EpMGSTex','Επίλυση μη γραμμικού συστήματος με τεχνάσματα');
INSERT INTO "Exercise_Types" VALUES ('EpalIsot','Επαλήθευση ισότητας - λύση εξίσωσης');
INSERT INTO "Exercise_Types" VALUES ('Epil2ouEP','Επίλυση εξίσωσης 2ου β. όταν β=0 ή γ=0');
INSERT INTO "Exercise_Types" VALUES ('EpilAnis','Επίλυση απλής πολυωνυμικής ανίσωσης');
INSERT INTO "Exercise_Types" VALUES ('EpilAnisS','Επίλυση ανισώσεων');
INSERT INTO "Exercise_Types" VALUES ('EpilExis','Επίλυση απλής πολυωνυμικής εξίσωσης');
INSERT INTO "Exercise_Types" VALUES ('EpilExisS','Επίλυση εξισώσεων');
INSERT INTO "Exercise_Types" VALUES ('EpilTrig','Επίλυση τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('EpilTyp','Επίλυση τύπου');
INSERT INTO "Exercise_Types" VALUES ('EpipHmi','Επίπεδο - Ημιεπίπεδο');
INSERT INTO "Exercise_Types" VALUES ('EplTheor','Επιλογή θεωρίας');
INSERT INTO "Exercise_Types" VALUES ('ErwTheor','Ερωτήσεις θεωρίας');
INSERT INTO "Exercise_Types" VALUES ('EswtGinom','Εσωτερικό γινόμενο διανυσμάτων');
INSERT INTO "Exercise_Types" VALUES ('EuAntPollT','Εύρεση αντίστροφης συνάρτησης πολλαπλού τύπου');
INSERT INTO "Exercise_Types" VALUES ('EuParMes','Ευθεία παράλληλη από μέσο πλευράς');
INSERT INTO "Exercise_Types" VALUES ('EurAntistr','Εύρεση αντίστροφης συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('EurTrigAr','Εύρεση τριγωνομετρικών αριθμών');
INSERT INTO "Exercise_Types" VALUES ('EurTypou','Εύρεση τύπου συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('EuthApod','Ευθεία απόδειξη');
INSERT INTO "Exercise_Types" VALUES ('EuthArith','Ευθεία των αριθμών');
INSERT INTO "Exercise_Types" VALUES ('EuthIdiot','Ευθεία με γνωστό σημείο - συντελεστή και ιδιότητα');
INSERT INTO "Exercise_Types" VALUES ('EuthKinish','Ευθύγραμμη κίνηση');
INSERT INTO "Exercise_Types" VALUES ('EuthTm','Ευθύγραμμο τμήμα - Μέσο τμήματος');
INSERT INTO "Exercise_Types" VALUES ('Eutheia','Ευθεία γραμμή');
INSERT INTO "Exercise_Types" VALUES ('ExEfKyk','Εξίσωση εφαπτομένης κύκλου (αρχή αξόνων + τυχαία θέση)');
INSERT INTO "Exercise_Types" VALUES ('ExEfapt','Εξίσωση εφαπτομένης');
INSERT INTO "Exercise_Types" VALUES ('ExEuthShm','Εξίσωση ευθείας με γνωστό σημείο (επί ή εκτός)');
INSERT INTO "Exercise_Types" VALUES ('ExEuthSynt','Εξίσωση ευθείας με γνωστό συντελεστή');
INSERT INTO "Exercise_Types" VALUES ('ExGwn','Σχέση εξωτερικής γωνίας με απέναντι γωνίες τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('ExKyklou','Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)');
INSERT INTO "Exercise_Types" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση');
INSERT INTO "Exercise_Types" VALUES ('ExisApolTim','Εξισώσεις με απόλυτες τιμές');
INSERT INTO "Exercise_Types" VALUES ('ExisXnA','Εξισώσεις x^n=a');
INSERT INTO "Exercise_Types" VALUES ('ExisXnAn','Εξισώσεις x^n=a^n');
INSERT INTO "Exercise_Types" VALUES ('ExwtGwn','Εξωτερική γωνία τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('Fapof2','Εύρεση συνάρτησης f από f^2');
INSERT INTO "Exercise_Types" VALUES ('GPAmbl','Γενικευμένο Πυθαγόρειο θεώρημα για αμβλεία γωνία');
INSERT INTO "Exercise_Types" VALUES ('GPOxeia','Γενικευμένο Πυθαγόρειο θεώρημα για οξεία γωνία');
INSERT INTO "Exercise_Types" VALUES ('GePrOrGin','Προσδιορισμός όρων γ.π. από γινόμενο');
INSERT INTO "Exercise_Types" VALUES ('GenExKykl','Γενική εξίσωση κύκλου');
INSERT INTO "Exercise_Types" VALUES ('GenOrArPr','Γενικός όρος αριθμητικής προόδου - Εύρεση όρων και βήματος ω');
INSERT INTO "Exercise_Types" VALUES ('GenOrGePr','Γενικός όρος γεωμετρικής προόδου - Εύρεση όρων και λόγου λ');
INSERT INTO "Exercise_Types" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές');
INSERT INTO "Exercise_Types" VALUES ('GeomMes','Γεωμετρικός μέσος - Διαδοχικοί όροι γεωμετρικής προόδου');
INSERT INTO "Exercise_Types" VALUES ('GeomParem','Γεωμετρική παρεμβολή');
INSERT INTO "Exercise_Types" VALUES ('GeomTopoi','Γεωμετρικοί τόποι');
INSERT INTO "Exercise_Types" VALUES ('GinomArith','Γινόμενο αριθμών');
INSERT INTO "Exercise_Types" VALUES ('GrParAnt','Γραφικές παραστάσεις των -f(x) - f(-x) - |f(x)|');
INSERT INTO "Exercise_Types" VALUES ('GrParDed','Γραφική παράσταση δεδομένων');
INSERT INTO "Exercise_Types" VALUES ('GrafEpil','Γραφική επίλυση συστήματος');
INSERT INTO "Exercise_Types" VALUES ('GramExis','Γραμμική εξίσωση - Λύση -  Σημείο - Τομή με άξονες - Σχεδίαση');
INSERT INTO "Exercise_Types" VALUES ('GrammSyn','Γραμμικός συνδυασμός διανυσμάτων');
INSERT INTO "Exercise_Types" VALUES ('GwnPEuth','Από γωνίες σε παράλληλες ευθείες');
INSERT INTO "Exercise_Types" VALUES ('GwnTrig','Γωνίες τριγώνου - πολυγώνου (προσκείμενες-περιεχόμενες)');
INSERT INTO "Exercise_Types" VALUES ('Gwnia','Γωνία - Ορισμός γωνίας');
INSERT INTO "Exercise_Types" VALUES ('GwniaDian','Γωνία διανυσμάτων');
INSERT INTO "Exercise_Types" VALUES ('GwniaEuth','Γωνία ευθειών');
INSERT INTO "Exercise_Types" VALUES ('Hmieuth','Ημιευθεία');
INSERT INTO "Exercise_Types" VALUES ('IdTetRiz','Ιδιότητες τετραγωνικής ρίζας');
INSERT INTO "Exercise_Types" VALUES ('IdiotAnis','Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων');
INSERT INTO "Exercise_Types" VALUES ('IdiotDynam','Ιδιότητες δυνάμεων');
INSERT INTO "Exercise_Types" VALUES ('IdiotEsGin','Ιδιότητες εσωτερικού γινομένου');
INSERT INTO "Exercise_Types" VALUES ('IdiotGin','Ιδιότητες γινομένου αριθμού με διάνυσμα');
INSERT INTO "Exercise_Types" VALUES ('IdiotIsot','Ιδιότητες ισοτήτων');
INSERT INTO "Exercise_Types" VALUES ('IdiotLog','Ιδιότητες λογαρίθμων');
INSERT INTO "Exercise_Types" VALUES ('IdiotNRiz','Ιδιότητες ν-οστής ρίζας');
INSERT INTO "Exercise_Types" VALUES ('IdiotOlokl','Ιδιότητες ορισμένου ολοκληρώματος');
INSERT INTO "Exercise_Types" VALUES ('IdiotOria','Ιδιότητες ορίων');
INSERT INTO "Exercise_Types" VALUES ('IdiotSymm','Ιδιότητες άρτιων και περιττών συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('IdiotSyxn','Ιδιότητες συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('IsaAntDian','Ίσα - Αντίθετα διανύσματα');
INSERT INTO "Exercise_Types" VALUES ('IsaMhdPar','Ίσα διανύσματα - Μηδενικό δ. - Παράλληλα με άξονες');
INSERT INTO "Exercise_Types" VALUES ('IsaSynol','Ίσα σύνολα');
INSERT INTO "Exercise_Types" VALUES ('IsesPar','Συναρτήσεις με ίσες παραγώγους σε διάστημα');
INSERT INTO "Exercise_Types" VALUES ('IsesParD','Συναρτήσεις με ίσες παραγώγους σε ένωση διαστημάτων');
INSERT INTO "Exercise_Types" VALUES ('IsesSynart','Ισότητα συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('IsodKlasm','Ισοδύναμα κλάσματα');
INSERT INTO "Exercise_Types" VALUES ('Isodynamia','Ισοδυναμία');
INSERT INTO "Exercise_Types" VALUES ('IsosTrap','Ισοσκελές τραπέζιο');
INSERT INTO "Exercise_Types" VALUES ('IsoskTrig','Ισοσκελή τρίγωνα');
INSERT INTO "Exercise_Types" VALUES ('IsotPolyon','Ισότητα πολυωνύμων');
INSERT INTO "Exercise_Types" VALUES ('IstogrSyxn','Ιστόγραμμα και πολύγωνο συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('KanDiair','Κανόνες διαιρετότητας');
INSERT INTO "Exercise_Types" VALUES ('KanonKat','Κανονική κατανομή');
INSERT INTO "Exercise_Types" VALUES ('KatPinaka','Κατασκευή πίνακα - Επεξεργασία δεδομένων');
INSERT INTO "Exercise_Types" VALUES ('KatakAsym','Κατακόρυφη ασύμπτωτη');
INSERT INTO "Exercise_Types" VALUES ('KatakMet','Κατακόρυφη μετατόπιση γραφικής παράστασης');
INSERT INTO "Exercise_Types" VALUES ('KatakorGwn','Κατακορυφήν γωνίες');
INSERT INTO "Exercise_Types" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος');
INSERT INTO "Exercise_Types" VALUES ('KathDian','Κάθετα διανύσματα');
INSERT INTO "Exercise_Types" VALUES ('KathPlTm','Κάθετα και πλάγια τμήματα');
INSERT INTO "Exercise_Types" VALUES ('Kathetes','Κάθετες ευθείες');
INSERT INTO "Exercise_Types" VALUES ('KenoSyn','Κενό σύνολο');
INSERT INTO "Exercise_Types" VALUES ('KinisKamp','Κίνηση σε καμπύλη');
INSERT INTO "Exercise_Types" VALUES ('KlaEx1ou','Εξισώσεις που ανάγονται σε 1ου βαθμού');
INSERT INTO "Exercise_Types" VALUES ('KlaEx2ou','Εξισώσεις που ανάγονται σε 2ου βαθμού');
INSERT INTO "Exercise_Types" VALUES ('KlaEx3ou','Εξισώσεις που ανάγονται σε 3ου βαθμού');
INSERT INTO "Exercise_Types" VALUES ('KlasAnis','Κλασματικές ανισώσεις');
INSERT INTO "Exercise_Types" VALUES ('KoinEf','Κοινές εφαπτομένες κύκλων');
INSERT INTO "Exercise_Types" VALUES ('KoinEfDS','Κοινή εφαπτομένη σε διαφορετικά σημεία');
INSERT INTO "Exercise_Types" VALUES ('KoinEfKS','Κοινή εφαπτομένη σε κοινό σημείο');
INSERT INTO "Exercise_Types" VALUES ('KoinLys','Κοινές λύσεις ανισώσεων και διπλών ανισώσεων');
INSERT INTO "Exercise_Types" VALUES ('KoinPar','Κοινός παράγοντας');
INSERT INTO "Exercise_Types" VALUES ('KoinesEfap','Κοινές εφαπτομένες');
INSERT INTO "Exercise_Types" VALUES ('KorParab','Κορυφή - άξονας παραβολής');
INSERT INTO "Exercise_Types" VALUES ('KritIsot','Κριτήρια ισότητας τριγώνων');
INSERT INTO "Exercise_Types" VALUES ('KritParem','Κριτήριο παρεμβολής');
INSERT INTO "Exercise_Types" VALUES ('Kwnos','Κώνος');
INSERT INTO "Exercise_Types" VALUES ('KybParEp','Κύβος - Παραλληλεπίπεδο - Πρίσμα');
INSERT INTO "Exercise_Types" VALUES ('KyklDiagr','Κυκλικό διάγραμμα συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('Kylindr','Κύλινδρος');
INSERT INTO "Exercise_Types" VALUES ('KyrStoix','Κύρια στοιχεία τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('KyrtEfapt','Κυρτότητα και εφαπτομένες');
INSERT INTO "Exercise_Types" VALUES ('KyrtPollT','Κυρτότητα - σημεία καμπής συν. πολλαπλού τύπου - απόλυτης τιμής');
INSERT INTO "Exercise_Types" VALUES ('LogEmIs','Λόγος εμβαδών τριγώνων με ίσες βάσεις - ύψη');
INSERT INTO "Exercise_Types" VALUES ('LogGwn','Λόγος εμβαδών τριγώνων με ίσες ή παραπλ. γωνίες');
INSERT INTO "Exercise_Types" VALUES ('LogOmP','Λόγος εμβαδών ομοίων τριγώνων - πολυγώνων');
INSERT INTO "Exercise_Types" VALUES ('LogarAnis','Λογαριθμικές ανισώσεις');
INSERT INTO "Exercise_Types" VALUES ('LogarExis','Λογαριθμικές εξισώσεις');
INSERT INTO "Exercise_Types" VALUES ('LogarSyst','Λογαριθμικά συστήματα');
INSERT INTO "Exercise_Types" VALUES ('MDOmad','Μέτρα διασποράς από ομαδοποιημένα δεδομένα');
INSERT INTO "Exercise_Types" VALUES ('MDParat','Μέτρα διασποράς από παρατηρήσεις');
INSERT INTO "Exercise_Types" VALUES ('MDSynx','Μέτρα διασποράς από πίνακς συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('MKDAr','Μ.Κ.Δ. αριθμών');
INSERT INTO "Exercise_Types" VALUES ('MParEnD','Μηδενική παράγωγος σε ένωση διαστημάτων');
INSERT INTO "Exercise_Types" VALUES ('MTDiamOm','Μέση τιμή και διάμεσος από ομαδοποιημένα δεδομένα');
INSERT INTO "Exercise_Types" VALUES ('MTDiamSyx','Μέση τιμή και διάμεσος από πίνακα συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('MeikKl','Μετατροπή μεταξύ μεικτού και κλάσματος');
INSERT INTO "Exercise_Types" VALUES ('MelKyrtot','Μελέτη συνάρτησης ως προς την κυρτότητα');
INSERT INTO "Exercise_Types" VALUES ('MelSynSym','Μελέτη συνάρτησης ως προς συμμετρία');
INSERT INTO "Exercise_Types" VALUES ('MelethSyn','Μελέτη συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('MesTimh','Μέση τιμή από παρατηρήσεις');
INSERT INTO "Exercise_Types" VALUES ('Mesokath','Μεσοκάτθετος τμήματος');
INSERT INTO "Exercise_Types" VALUES ('MetIsod','Μετατροπή ισοδύναμου κλάσματος');
INSERT INTO "Exercise_Types" VALUES ('MethAnt','Μέθοδος αντικατάστασης');
INSERT INTO "Exercise_Types" VALUES ('MethAntS','Μέθοδος αντίθετων συντελεστών');
INSERT INTO "Exercise_Types" VALUES ('MethOriz','Μέθοδος οριζουσών');
INSERT INTO "Exercise_Types" VALUES ('MetrGrSyn','Μέτρο γραμμικού συνδυασμού');
INSERT INTO "Exercise_Types" VALUES ('MetrGwn','Μέτρο - σύγκριση γωνιών');
INSERT INTO "Exercise_Types" VALUES ('MetroDian','Μέτρο διανύσματος');
INSERT INTO "Exercise_Types" VALUES ('Metyax','Η μεταβλητή y=ax+b - Μεταβολές παρατηρήσεων');
INSERT INTO "Exercise_Types" VALUES ('MhdFragm','Όριο μηδενικής επί φραγμένης');
INSERT INTO "Exercise_Types" VALUES ('MhdOlok','Μηδενικό ολοκλήρωμα');
INSERT INTO "Exercise_Types" VALUES ('MhkKenAkt','Μήκος - κέντρο και ακτίνα διαστήματος');
INSERT INTO "Exercise_Types" VALUES ('MhkKykl','Μήκος κύκλου');
INSERT INTO "Exercise_Types" VALUES ('MhkTox','Μήκος τόξου');
INSERT INTO "Exercise_Types" VALUES ('MhkosTm','Μήκος - σύγκριση ευθυγράμμων τμημάτων');
INSERT INTO "Exercise_Types" VALUES ('MidAthr','Μηδενικό άθροισμα');
INSERT INTO "Exercise_Types" VALUES ('MidGinom','Μηδενικό και μη μηδενικό γινόμενο');
INSERT INTO "Exercise_Types" VALUES ('MoirAkt','Μετατροπή μοιρών σε ακτίνια και αντίστροφα');
INSERT INTO "Exercise_Types" VALUES ('MonAkrGrPar','Μονοτονία και ακρότατα από γραφική παράσταση');
INSERT INTO "Exercise_Types" VALUES ('MonAnt','Μονοτονία αντίστροφης συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('MonPolPar','Μονοτονία με χρήση 2ης - 3ης παραγώγου - βοηθητικής');
INSERT INTO "Exercise_Types" VALUES ('MonPollT','Μονοτονία ακρότητα συν. πολλαπλού τύπου - Απόλυτη τιμή');
INSERT INTO "Exercise_Types" VALUES ('MonadRiz','Μοναδική ρίζα');
INSERT INTO "Exercise_Types" VALUES ('MonotPar','Μελέτη μονοτονίας με τη χρήση παραγώγου');
INSERT INTO "Exercise_Types" VALUES ('Monoton','Μελέτη μονοτονίας συνάρτησης με ορισμό');
INSERT INTO "Exercise_Types" VALUES ('NRizes','Ύπαρξη ν ριζών');
INSERT INTO "Exercise_Types" VALUES ('NomHmit','Νόμος ημιτόνων');
INSERT INTO "Exercise_Types" VALUES ('NomosSyn','Νόμος συνημιτόνων');
INSERT INTO "Exercise_Types" VALUES ('OikEutheiwn','Οικογένεια ευθειών');
INSERT INTO "Exercise_Types" VALUES ('OlokAnis','Ορισμένο ολοκλήρωμα και ανισότητες');
INSERT INTO "Exercise_Types" VALUES ('OlokAntik','Ολοκλήρωση με αντικατάσταση');
INSERT INTO "Exercise_Types" VALUES ('OlokAntist','Ολοκλήρωμα αντίστροφης συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('OlokArPer','Ολοκλήρωμα άρτιας - περιττής και περιοδικής');
INSERT INTO "Exercise_Types" VALUES ('OlokOria','Ολοκλήρωμα και όρια');
INSERT INTO "Exercise_Types" VALUES ('OlokParag','Παραγοντική ολοκλήρωση');
INSERT INTO "Exercise_Types" VALUES ('OlokPollT','Ολοκλήρωμα συνάρτησης πολλαπλού τύπου - απόλυτης τιμής');
INSERT INTO "Exercise_Types" VALUES ('OlokRhtwn','Ολοκληρώματα ρητών συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('OlokSynd','Ολοκλήρωμα μέσα σε ολοκλήρωμα');
INSERT INTO "Exercise_Types" VALUES ('OmDeutSt','Όμοια τρίγωνα και δευτερεύοντα στοιχεία');
INSERT INTO "Exercise_Types" VALUES ('OmEterAr','Ομόσημοι και ετερόσημοι αριθμοί');
INSERT INTO "Exercise_Types" VALUES ('OmEterKl','Ομόνυμα - ετερώνυμα κλάσματα');
INSERT INTO "Exercise_Types" VALUES ('OmIsaAntM','Όμοια - Ίσα - Αντίθετα μονώνυμα');
INSERT INTO "Exercise_Types" VALUES ('OmKanPol','Κανονικά πολύγωνα και ομοιότητα');
INSERT INTO "Exercise_Types" VALUES ('OmSxhm','Όμοια σχήματα');
INSERT INTO "Exercise_Types" VALUES ('Omadop','Ομαδοποίηση');
INSERT INTO "Exercise_Types" VALUES ('OmoiaTr','Κριτήρια ομοιότητας τριγώνων');
INSERT INTO "Exercise_Types" VALUES ('Or0epiAp','Απροσδιοριστία 0 επί άπειρο');
INSERT INTO "Exercise_Types" VALUES ('Or0p0AppAp','Απροσδιοριστία 0 προς 0 ή άπειρο προς άπειρο');
INSERT INTO "Exercise_Types" VALUES ('Or0p0PTAp','Απροσδιοριστία 0 προς 0 πολλαπλού τύπου - απόλυτες τιμές');
INSERT INTO "Exercise_Types" VALUES ('Or0p0Riz','Απροσδιοριστία 0 προς 0 άρρητης');
INSERT INTO "Exercise_Types" VALUES ('Or1p0ap0','Όρια της μορφής 1 προς 0 και α προς 0');
INSERT INTO "Exercise_Types" VALUES ('Or1p1','Ορισμός συνάρτησης 1 - 1');
INSERT INTO "Exercise_Types" VALUES ('OrApTim','Όριο με απόλυτες τιμές');
INSERT INTO "Exercise_Types" VALUES ('OrApolTim','Ορισμός απόλυτης τιμής');
INSERT INTO "Exercise_Types" VALUES ('OrAprEkth','Απροσδιοριστία εκθετικής μορφής');
INSERT INTO "Exercise_Types" VALUES ('OrArrAp','Όριο άρρητης συνάρτησης στο άπειρο');
INSERT INTO "Exercise_Types" VALUES ('OrEkthAp','Όριο εκθετικής συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('OrGrPar','Ορισμός γρ. Παράστασης - Σημείο που ανήκει στη γρ. παρ');
INSERT INTO "Exercise_Types" VALUES ('OrKatMet','Συνδυασμός μετατοπίσεων');
INSERT INTO "Exercise_Types" VALUES ('OrLogAp','Όριο λογαριθμικής συνάρτησης στο άπειρο');
INSERT INTO "Exercise_Types" VALUES ('OrPolyon','Όριο πολυωνυμικής - ρητής συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('Orapmap','Απροσδιοριστία άπειρο - άπειρο');
INSERT INTO "Exercise_Types" VALUES ('OriaPraks','Όρια και πράξεις - Υπολογισμός');
INSERT INTO "Exercise_Types" VALUES ('Orio0p0Pol','Απροσδιοριστία 0 προς 0 ρητής');
INSERT INTO "Exercise_Types" VALUES ('OrioAnis','Υπολογισμός ορίου από ανισότητα');
INSERT INTO "Exercise_Types" VALUES ('OrioGrPar','Όριο συνάρτησης από γραφική παράσταση');
INSERT INTO "Exercise_Types" VALUES ('OrioSynth','Όριο σύνθετης - Μέθοδος αλλαγής μεταβλητής');
INSERT INTO "Exercise_Types" VALUES ('OrisExis','Ορισμός εξίσωσης');
INSERT INTO "Exercise_Types" VALUES ('OrisRMet','Ορισμός ρυθμού μεταβολής');
INSERT INTO "Exercise_Types" VALUES ('OrisRizas','Ορισμός ρίζας');
INSERT INTO "Exercise_Types" VALUES ('OrisSyn','Ορισμός συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('OrismAkol','Ορισμός ακολουθίας');
INSERT INTO "Exercise_Types" VALUES ('OrismArPr','Ορισμός αριθμητικής προόδου');
INSERT INTO "Exercise_Types" VALUES ('OrismGePr','Ορισμός γεωμετρικής προόδου');
INSERT INTO "Exercise_Types" VALUES ('OrismMon','Ορισμός μονωνύμου');
INSERT INTO "Exercise_Types" VALUES ('OrismPol','Ορισμός πολυωνύμου');
INSERT INTO "Exercise_Types" VALUES ('OrizAsym','Οριζόντια ασύμπτωτη');
INSERT INTO "Exercise_Types" VALUES ('OrizMet','Οριζόντια μετατόπιση γραφικής παράστασης');
INSERT INTO "Exercise_Types" VALUES ('OrthAmb','Ορθογώνια και αμβλυγώνια τρίγωνα');
INSERT INTO "Exercise_Types" VALUES ('OrthProb','Θεώρημα προβολών');
INSERT INTO "Exercise_Types" VALUES ('Orthog','Ορθογώνιο');
INSERT INTO "Exercise_Types" VALUES ('PEuGwn','Από παράλληλες ευθείες σε γωνίες');
INSERT INTO "Exercise_Types" VALUES ('POrRhtP','Πεδίο ορισμού ρητής παράστασης');
INSERT INTO "Exercise_Types" VALUES ('ParAnTax','Παράγωγοι ανώτερης τάξης');
INSERT INTO "Exercise_Types" VALUES ('ParAnis','Υπολογισμός παραγώγου από ανισότητα');
INSERT INTO "Exercise_Types" VALUES ('ParAntistr','Παράγωγος αντίστροφης συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('ParArtPer','Παράγωγος άριας και περιττής συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('ParEuIsT','Παράλληλες ευθείες που ορίζουν ίσα τμήματα');
INSERT INTO "Exercise_Types" VALUES ('ParIsSyn','Ισότητα της μορφής f(x)=f(x)');
INSERT INTO "Exercise_Types" VALUES ('ParKathDian','Παράλληλο ή κάθετο διάνυσμα σε ευθεία');
INSERT INTO "Exercise_Types" VALUES ('ParPollTyp','Παράγωγος συνάρτησης πολλαπλού τύπου - απόλυτες τιμές');
INSERT INTO "Exercise_Types" VALUES ('ParSynEkth','Παράγωγος σύνθετης εκθετικής');
INSERT INTO "Exercise_Types" VALUES ('ParSynex','Παράγωγος και συνέχεια');
INSERT INTO "Exercise_Types" VALUES ('ParSynthRiz','Παράγωγος σύνθετης ν-οστής ρίζας');
INSERT INTO "Exercise_Types" VALUES ('ParagAth','Παράγωγος αθροίσματος');
INSERT INTO "Exercise_Types" VALUES ('ParagEx','Επίλυση εξίσωσης με παραγοντοποίηση');
INSERT INTO "Exercise_Types" VALUES ('ParagGin','Παράγωγος γινομένου');
INSERT INTO "Exercise_Types" VALUES ('ParagOlok','Παράγωγος');
INSERT INTO "Exercise_Types" VALUES ('ParagOria','Παράγωγοι και όρια');
INSERT INTO "Exercise_Types" VALUES ('ParagPilik','Παράγωγος πηλίκου');
INSERT INTO "Exercise_Types" VALUES ('ParagShm','Παράγωγος συνάρτησης σε σημείο');
INSERT INTO "Exercise_Types" VALUES ('ParagSynth','Παράγωγος σύνθετης συνάρτησης (Πίνακας)');
INSERT INTO "Exercise_Types" VALUES ('ParagTriwn','Παραγοντοποίηση τριωνύμου');
INSERT INTO "Exercise_Types" VALUES ('ParallKath','Συνθήκη παραλληλίας - καθετότητας ευθειών');
INSERT INTO "Exercise_Types" VALUES ('ParallTemn','Παράλληλες - Τεμνόμενες ευθείες');
INSERT INTO "Exercise_Types" VALUES ('Parallhl','Παραλληλόγραμμο');
INSERT INTO "Exercise_Types" VALUES ('Parametrikes','Παραμετρικές');
INSERT INTO "Exercise_Types" VALUES ('ParaplGwn','Παραπληρωματικές γωνίες');
INSERT INTO "Exercise_Types" VALUES ('ParegKykl','Παρεγγεγραμμένος κύκλος');
INSERT INTO "Exercise_Types" VALUES ('PedioOrism','Πεδίο ορισμού συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('PerigKykl','Περιγεγραμμένος κύκλος');
INSERT INTO "Exercise_Types" VALUES ('PerigrSyn','Περιγραφή συνόλου');
INSERT INTO "Exercise_Types" VALUES ('PeriodSyn','Έλεγχος περιοδικότητας - Εύρεση περιόδου');
INSERT INTO "Exercise_Types" VALUES ('Pinakas','Συμπλήρωση πίνακα');
INSERT INTO "Exercise_Types" VALUES ('PinakesAl','Πίνακες αληθείας');
INSERT INTO "Exercise_Types" VALUES ('PlRizSTim','Πλήθος ριζών - Σύνολο τιμών συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('PlagAsym','Πλάγια ασύμπτωτη');
INSERT INTO "Exercise_Types" VALUES ('PleurOr','Πλευρικά όρια συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('PollEpil','Πολλαπλής Επιλογής');
INSERT INTO "Exercise_Types" VALUES ('PollKl','Πολλαπλασιασμός κλασμάτων - ακέραιου και κλάσματος');
INSERT INTO "Exercise_Types" VALUES ('Pollex','Πολλαπλασιασμός με e^x');
INSERT INTO "Exercise_Types" VALUES ('Polygwna','Πολύγωνα (βασικές έννοιες)');
INSERT INTO "Exercise_Types" VALUES ('PrAkKl','Πρόσθεση ακέραιου με κλάσμα');
INSERT INTO "Exercise_Types" VALUES ('PrEmbOgk','Προβλήματα εμβαδού - όγκου');
INSERT INTO "Exercise_Types" VALUES ('PrFysikhs','Προβλήματα φυσικής');
INSERT INTO "Exercise_Types" VALUES ('PrKlasm','Πρόσθεση - αφαίρεση κλασμάτων');
INSERT INTO "Exercise_Types" VALUES ('PrOikonom','Προβλήματα οικονομίας');
INSERT INTO "Exercise_Types" VALUES ('PrRhtPar','Πράξεις μεταξύ ρητών παραστάσεων');
INSERT INTO "Exercise_Types" VALUES ('PraksDekAr','Πράξεις δεκαδικών αριθμών');
INSERT INTO "Exercise_Types" VALUES ('PraksMon','Πράξεις μεταξύ μονωνύμων');
INSERT INTO "Exercise_Types" VALUES ('PraksPol','Πράξεις μεταξύ πολυωνύμων');
INSERT INTO "Exercise_Types" VALUES ('PraksSyn','Πράξεις μεταξύ συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('PraksSynol','Πράξεις συνόλων');
INSERT INTO "Exercise_Types" VALUES ('ProbDian','Προβολή διανύσματος - Συνιστώσες');
INSERT INTO "Exercise_Types" VALUES ('Problhmata','Προβλήματα');
INSERT INTO "Exercise_Types" VALUES ('ProsShm','Προσδιορισμός σημείου');
INSERT INTO "Exercise_Types" VALUES ('ProsSyn','Πρόσημο συνεχούς συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('ProsTrigAr','Πρόσημο τριγωνομετρικού αριθμού σε κάθε τεταρτημόριο');
INSERT INTO "Exercise_Types" VALUES ('ProsTriwn','Πρόσημο τριωνύμου');
INSERT INTO "Exercise_Types" VALUES ('ProsdEuth','Προσδιορισμός ευθείας');
INSERT INTO "Exercise_Types" VALUES ('ProsdPol','Προσδιορισμός πολυωνύμου');
INSERT INTO "Exercise_Types" VALUES ('ProsdTim','Προσδιορισμός τιμής από συνέχεια');
INSERT INTO "Exercise_Types" VALUES ('ProsthAr','Πρόσθεση αριθμών');
INSERT INTO "Exercise_Types" VALUES ('ProsthDian','Πρόσθεση - Αφαίρεση διανυσμάτων');
INSERT INTO "Exercise_Types" VALUES ('ProsthTm','Πρόσθεση ευθυγράμμων τμημάτων');
INSERT INTO "Exercise_Types" VALUES ('PrwtoiAr','Πρώτοι αριθμοί - Πρώτοι μεταξύ τους');
INSERT INTO "Exercise_Types" VALUES ('Pyramida','Πυραμίδα');
INSERT INTO "Exercise_Types" VALUES ('PythThewr','Πυθαγόρειο θεώρημα');
INSERT INTO "Exercise_Types" VALUES ('RMetGwn','Ρυθμός μεταβολής και γωνία');
INSERT INTO "Exercise_Types" VALUES ('RMetOmTr','Ρυθμός μεταβολής και όμοια τρίγωνα');
INSERT INTO "Exercise_Types" VALUES ('Rabdogr','Ραβρόγραμμα συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('RhtopPar','Ρητοποίηση παρονομαστή');
INSERT INTO "Exercise_Types" VALUES ('RizAnDiast','Ύπαρξη ρίζας σε ανοιχτό διάστημα');
INSERT INTO "Exercise_Types" VALUES ('RizKlDiast','Ύπαρξη ρίζας σε κλειστό διάστημα');
INSERT INTO "Exercise_Types" VALUES ('RizaExis','Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας');
INSERT INTO "Exercise_Types" VALUES ('RolleArx','Θ. Rolle με αρχική συνάρτηση');
INSERT INTO "Exercise_Types" VALUES ('Rombos','Ρόμβος');
INSERT INTO "Exercise_Types" VALUES ('Sfaira','Σφαίρα');
INSERT INTO "Exercise_Types" VALUES ('ShmKamp','Σημεία καμπής γραφικής παράστασης');
INSERT INTO "Exercise_Types" VALUES ('ShmTom','Σημεία τομής ευθειών - Τομή με άξονες - Σημεία σε γραμμή');
INSERT INTO "Exercise_Types" VALUES ('ShmXron','Σημειόγραμμα - Χρονόγραμμα συχνοτήτων');
INSERT INTO "Exercise_Types" VALUES ('StSynGrPar','Στοιχεία από γραφική παράσταση');
INSERT INTO "Exercise_Types" VALUES ('StahPrSyn','Συνάρτηση με σταθερό πρόσημο');
INSERT INTO "Exercise_Types" VALUES ('StatSyn','Σταθερή συνάρτηση - Εύρεση τύπου');
INSERT INTO "Exercise_Types" VALUES ('StathMesos','Σταθμικός μέσος');
INSERT INTO "Exercise_Types" VALUES ('StathPros','Υπολογισμός απόλυτης τιμής με παράσταση σταθερού προσήμου');
INSERT INTO "Exercise_Types" VALUES ('StoixEll','Στοιχεία έλλειψης (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)');
INSERT INTO "Exercise_Types" VALUES ('StoixParab','Στοιχεία παραβολής (Εξίσωση-εστία-διευθετούσα-παράμετρος p)');
INSERT INTO "Exercise_Types" VALUES ('StoixPol','Στοιχεία κανονικού πολυγώνου (γωνία-κεντρική-πλευρά-περίμετρος-εμβαδόν)');
INSERT INTO "Exercise_Types" VALUES ('StoixTrig','Στοιχεία τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('StoixYperb','Στοιχεία υπερβολής (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)');
INSERT INTO "Exercise_Types" VALUES ('SwstoLathos','Σωστό - Λάθος');
INSERT INTO "Exercise_Types" VALUES ('SxThEKyk','Σχετική θέση ευθείας και κύκλου');
INSERT INTO "Exercise_Types" VALUES ('SxThEpip','Σχετικές θέσεις επιπέδων');
INSERT INTO "Exercise_Types" VALUES ('SxThEuEp','Σχετική θέση ευθείας και επιπεέδου');
INSERT INTO "Exercise_Types" VALUES ('SxThEuth','Σχετική θέση ευθειών - Ασύμβατες ευθείες');
INSERT INTO "Exercise_Types" VALUES ('SxThGrPar','Σημεία τομής - Σχετική θέση γρ. παραστάσεων - Τομή με άξονες');
INSERT INTO "Exercise_Types" VALUES ('SxThKykl','Σχετική θέση κύκλων');
INSERT INTO "Exercise_Types" VALUES ('SxesOriz','Σχέσεις οριζουσών');
INSERT INTO "Exercise_Types" VALUES ('SxetThEuth','Σχετική θέση ευθειών');
INSERT INTO "Exercise_Types" VALUES ('SxhmTeth','Σχήμα - Τεθλασμένη γραμμή');
INSERT INTO "Exercise_Types" VALUES ('SxhmaHor','Σχήμα Horner');
INSERT INTO "Exercise_Types" VALUES ('SygkDekAr','Σύγκριση δεκαδικών αριθμών');
INSERT INTO "Exercise_Types" VALUES ('SygkFysAr','Σύγκριση φυσικών αριθμών');
INSERT INTO "Exercise_Types" VALUES ('SygkKlasm','Σύγκριση κλασμάτων');
INSERT INTO "Exercise_Types" VALUES ('SygkrAkKl','Σύγκριση ακεραίου με κλάσμα');
INSERT INTO "Exercise_Types" VALUES ('SygkrAr','Σύγκριση αριθμών');
INSERT INTO "Exercise_Types" VALUES ('SygkrRiz','Σύγκριση άρρητων παραστάσεων');
INSERT INTO "Exercise_Types" VALUES ('SygrAkAr','Σύγκριση ακέραιων αριθμών');
INSERT INTO "Exercise_Types" VALUES ('SymmPar','Συμμετρικές παραστάσεις ριζών x1-x2');
INSERT INTO "Exercise_Types" VALUES ('SymmTet','Συμμετρίες - Τεταρτημόρια');
INSERT INTO "Exercise_Types" VALUES ('SymplGwn','Συμπληρωματικές γωνίες');
INSERT INTO "Exercise_Types" VALUES ('SymplKenou','Συμπλήρωση κενού');
INSERT INTO "Exercise_Types" VALUES ('SynGwnD','Συνημίτονο γωνίας διανυσμάτων');
INSERT INTO "Exercise_Types" VALUES ('SynParall','Συνθήκη παραλληλίας διανυσμάτων');
INSERT INTO "Exercise_Types" VALUES ('SynPollTyp','Συναρτήσεις πολλαπλού τύπου');
INSERT INTO "Exercise_Types" VALUES ('SynSxesi','Συναρτησιακή σχέση');
INSERT INTO "Exercise_Types" VALUES ('SynartSx','Συναρτησιακές σχέσεις');
INSERT INTO "Exercise_Types" VALUES ('SyndMOl','Συνδυασμός μεθόδων ολοκλήρωσης');
INSERT INTO "Exercise_Types" VALUES ('Synepag','Συνεπαγωγή');
INSERT INTO "Exercise_Types" VALUES ('SynexShm','Συνέχεια συνάρτησης σε σημείο');
INSERT INTO "Exercise_Types" VALUES ('SynexSyn','Συνέχεια συνάρτησης σε σύνολο');
INSERT INTO "Exercise_Types" VALUES ('SyntDian','Συντεταγμένες διανύσματος');
INSERT INTO "Exercise_Types" VALUES ('SyntDieuth','Συντελεστής διεύθυνσης ευθείας - Γωνία');
INSERT INTO "Exercise_Types" VALUES ('SyntGnAk','Συντεταγμένες διαν. με γνωστά άκρα');
INSERT INTO "Exercise_Types" VALUES ('SyntGrSynd','Συντεταγμένες γραμμικού συνδυασμού');
INSERT INTO "Exercise_Types" VALUES ('SyntMes','Συντεταγμένες μέσου τμήματος - βαρύκεντρου');
INSERT INTO "Exercise_Types" VALUES ('SyntelDieu','Συντελεστής διεύθυνσης διανύσματος');
INSERT INTO "Exercise_Types" VALUES ('Syntetag','Συντεταγμένες σημείων');
INSERT INTO "Exercise_Types" VALUES ('SynthKl','Σύνθετα κλάσματα');
INSERT INTO "Exercise_Types" VALUES ('SynthSyn','Σύνθεση συναρτήσεων');
INSERT INTO "Exercise_Types" VALUES ('SynthSys','Σύνθετα συστήματα');
INSERT INTO "Exercise_Types" VALUES ('SynthTrEx','Σύνθετες τριγωνομετρικές εξισώσεις');
INSERT INTO "Exercise_Types" VALUES ('Syst3x3','Συστήματα 3x3');
INSERT INTO "Exercise_Types" VALUES ('Syzeuxh','Σύζευξη');
INSERT INTO "Exercise_Types" VALUES ('TaytSynth','Ταυτότητες υπό συνθήκη');
INSERT INTO "Exercise_Types" VALUES ('TeleiaDiair','Τέλεια διαίρεση πολυωνύμων');
INSERT INTO "Exercise_Types" VALUES ('TemnKykl','Τέμνουσες κύκλου - Τέμνουσα και εφαπτομένη');
INSERT INTO "Exercise_Types" VALUES ('Tetragwno','Τετράγωνο');
INSERT INTO "Exercise_Types" VALUES ('ThEsDix','Θεώρημα εσωτερική διχοτόμου - Τμήματα απέναντι πλευράς');
INSERT INTO "Exercise_Types" VALUES ('ThExDix','Θεώρημα εξωτερικής διχοτόμου - Τμήματα απέναντι πλευράς');
INSERT INTO "Exercise_Types" VALUES ('ThFermat','Θεώρημα Fermat');
INSERT INTO "Exercise_Types" VALUES ('ThMT+','Συνδυασμός Θ.Μ.Τ. με άλλα θεωρήματα');
INSERT INTO "Exercise_Types" VALUES ('ThThalh','Θεώρημα Θαλή');
INSERT INTO "Exercise_Types" VALUES ('ThYpsous','Θεώρημα ύψους προς την υποτείνουσα');
INSERT INTO "Exercise_Types" VALUES ('TheorAkRiz','Σχήμα Horner - Θεώρημα ακέραιων ριζών');
INSERT INTO "Exercise_Types" VALUES ('TheorRiz','Θεώρημα ρίζας πολυωνύμου');
INSERT INTO "Exercise_Types" VALUES ('TheorYpol','Θεώρημα υπολοίπου');
INSERT INTO "Exercise_Types" VALUES ('ThewrYp','Θεωρήματα ύπαρξης');
INSERT INTO "Exercise_Types" VALUES ('Thewritikes','Θεωρητικές');
INSERT INTO "Exercise_Types" VALUES ('TimhMon','Τιμή μονωνύμου');
INSERT INTO "Exercise_Types" VALUES ('TimhParast','Τιμή παράστασης - Σειρά πράξεων');
INSERT INTO "Exercise_Types" VALUES ('TimhPol','Τιμή - Ρίζα πολυωνύμου');
INSERT INTO "Exercise_Types" VALUES ('TimhSyn','Τιμή συνάρτησης');
INSERT INTO "Exercise_Types" VALUES ('TmMesaPl','Τμήμα που ενώνει τα μέσα πλευρών + αντίστροφο');
INSERT INTO "Exercise_Types" VALUES ('TrAr2pi+','Τριγωνομετρικοί αριθμοί γωνιών μεγαλύτερων του κύκλου');
INSERT INTO "Exercise_Types" VALUES ('TrArAth','Τριγωνομετρικοί αριθμοί αθροίσματος - διαφοράς');
INSERT INTO "Exercise_Types" VALUES ('TrArDipl','Τριγωνομετρικοί αριθμοί διπλάσιας γωνίας');
INSERT INTO "Exercise_Types" VALUES ('TrArTrig','Τριγωνομετρικοί αριθμοί γωνιών τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('TrArTripl','Τριγωνομετρικοί αριθμοί τριπλάσιας γωνίας');
INSERT INTO "Exercise_Types" VALUES ('TrExArnAr','Τριγωνομετρική εξίσωση με αρνητικό αριθμό');
INSERT INTO "Exercise_Types" VALUES ('TrExDiast','Τριγωνομετρική εξίσωση σε διάστημα');
INSERT INTO "Exercise_Types" VALUES ('Trapez','Τραπέζιο');
INSERT INTO "Exercise_Types" VALUES ('TrigAnis','Τριγωνική ανισότητα - Κριτήριο ομόρροπων');
INSERT INTO "Exercise_Types" VALUES ('TrigAntik','Ολοκλήρωση με τριγωνομετρική αντικατάσταση');
INSERT INTO "Exercise_Types" VALUES ('TrigBasGwn','Τριγωνομετρικοί αριθμοί βασικών γωνιών');
INSERT INTO "Exercise_Types" VALUES ('TrigEx','Επίλυση τριγωνομετρικής εξίσωσης');
INSERT INTO "Exercise_Types" VALUES ('TrigOlokl','Τριγωνομετρικά ολοκληρώματα');
INSERT INTO "Exercise_Types" VALUES ('TrigOrTr','Τριγωνομετρικοί αριθμοί σε ορθογώνιο τρίγωνο');
INSERT INTO "Exercise_Types" VALUES ('TrigOria','Τριγωνομετρικά όρια');
INSERT INTO "Exercise_Types" VALUES ('TrigSysSyn','Τριγωνομετρικοί αριθμοί σε σύστημα συντεταγμένων');
INSERT INTO "Exercise_Types" VALUES ('TrigonAnis','Τριγωνική ανισότητα');
INSERT INTO "Exercise_Types" VALUES ('TypoiVieta','Τύποι του Vieta');
INSERT INTO "Exercise_Types" VALUES ('XarGrafPar','Χάραξη γραφικής παράστασης');
INSERT INTO "Exercise_Types" VALUES ('XorToxAp','Χορδές - Τόξα - Αποστήματα κύκλου');
INSERT INTO "Exercise_Types" VALUES ('XordApost','Χορδές και αποστήματα κύκλου');
INSERT INTO "Exercise_Types" VALUES ('XordToxa','Χορδές και τόξα κύκλου');
INSERT INTO "Exercise_Types" VALUES ('XwrDiast','Χωρισμός διαστήματος σε υποδιαστήματα');
INSERT INTO "Exercise_Types" VALUES ('YpOrOlokl','Υπολογισμός ορισμένου ολοκληρώματος');
INSERT INTO "Exercise_Types" VALUES ('YpolLog','Υπολογισμός λογαρίθμων');
INSERT INTO "Exercise_Types" VALUES ('YpolParast','Υπολογισμός παραστάσεων');
INSERT INTO "Exercise_Types" VALUES ('Yposynolo','Υποσύνολο');
INSERT INTO "Exercise_Types" VALUES ('YpshTr','Ύψη τριγώνου');
INSERT INTO "Exercise_Types" VALUES ('YpΝRiz','Ύπαρξη ν ριζών');
INSERT INTO "Exercise_Types" VALUES ('Theoria','Θεωρία');
INSERT INTO "Exercise_Types" VALUES ('-','-');
INSERT INTO "Exercise_Types" VALUES ('Probl','Προβλήματα');
INSERT INTO "Exercise_Types" VALUES ('PrEnd','Πράξεις ενδεχομένων');
INSERT INTO "Exercise_Types" VALUES ('ThemaA','Θέμα Α');
INSERT INTO "Exercise_Types" VALUES ('ThemaB','Θέμα Β');
INSERT INTO "Exercise_Types" VALUES ('ThemaC','Θέμα Γ');
INSERT INTO "Exercise_Types" VALUES ('ThemaD','Θέμα Δ');
INSERT INTO "Exercise_Types" VALUES ('TheoriaSub','Θεωρία');
INSERT INTO "Exercise_Types" VALUES ('Ypolog','Υπολογιστικό');
INSERT INTO "Exercise_Types" VALUES ('Synd','Συνδυαστικό');
INSERT INTO "Exercise_Types" VALUES ('Synth','Σύνθετο');
INSERT INTO "Exercise_Types" VALUES ('OrSys','Ορισμός συστήματος');
INSERT INTO "Sections" VALUES ('Akolouth','Ακολουθίες','Algebra','Akol');
INSERT INTO "Sections" VALUES ('Anag1oTet','Αναγωγή στο 1ο τεταρτημόριο','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('Anis1ou','Ανισώσεις 1ου βαθμού','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('Anis2ou','Ανισώσεις 2ου βαθμού','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('Anis3ou+','Ανισώσεις 3ου+ βαθμού - Γινόμενο','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('ApolTimh','Απόλυτη τιμή','Algebra','Arith');
INSERT INTO "Sections" VALUES ('ArProodos','Αριθμητική πρόοδος','Algebra','Akol');
INSERT INTO "Sections" VALUES ('Arithmoi','Αριθμοί','Algebra','Arith');
INSERT INTO "Sections" VALUES ('ArritesAnEx','Άρρητες εξισώσεις - ανισώσεις','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('GeomProodos','Γεωμετρική πρόοδος','Algebra','Akol');
INSERT INTO "Sections" VALUES ('GrammikaSys','Γραμμικά συστήματα','Algebra','Syst');
INSERT INTO "Sections" VALUES ('DiaireshKlasm','Διαίρεση κλασμάτων','Algebra','Arith');
INSERT INTO "Sections" VALUES ('DiaireshPolyon','Διαίρεση πολυωνύμων','Algebra','AlgPar');
INSERT INTO "Sections" VALUES ('Diataxh','Διάταξη αριθμών','Algebra','Arith');
INSERT INTO "Sections" VALUES ('DionExiswseis','Διωνυμικές εξισώσεις ν- οστού βαθμού','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('Dynameis','Δυνάμεις πραγματικών αριθμών','Algebra','Arith');
INSERT INTO "Sections" VALUES ('EKPAlgPar','Ε.Κ.Π. - Μ.Κ.Δ. αλγεβρικών παραστάσεων','Algebra','AlgPar');
INSERT INTO "Sections" VALUES ('Exis1ou','Εξισώσεις 1ου βαθμού','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('Exis2ou','Εξισώσεις 2ου βαθμού','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('Exis3ou+','Εξισώσεις 3ου+ βαθμού - Γινόμενο','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('IsothtaKlasm','Ισότητα κλασμάτων','Algebra','Arith');
INSERT INTO "Sections" VALUES ('Klasmata','Κλάσματα','Algebra','Arith');
INSERT INTO "Sections" VALUES ('KlasmExAnis','Κλασματικές εξισώσεις - ανισώσεις','Algebra','ExAn');
INSERT INTO "Sections" VALUES ('Logarithmos','Λογάριθμος','Algebra','Logar');
INSERT INTO "Sections" VALUES ('Logikh','Λογική','Algebra','Logik');
INSERT INTO "Sections" VALUES ('MhGramSys','Μη γραμμικά συστήματα','Algebra','Syst');
INSERT INTO "Sections" VALUES ('Mononyma','Μονώνυμα','Algebra','AlgPar');
INSERT INTO "Sections" VALUES ('NomosHmSyn','Νόμος ημιτόνων και συνημιτότων','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('Paragont','Παραγοντοποίηση','Algebra','AlgPar');
INSERT INTO "Sections" VALUES ('EKP-MKD','Πολλαπλάσια - Διαιρέτες - ΕΚΠ - ΜΚΔ - Πρώτοι αριθμοί','Algebra','Arith');
INSERT INTO "Sections" VALUES ('PollKlasm','Πολλαπλασιασμός κλασμάτων','Algebra','Arith');
INSERT INTO "Sections" VALUES ('Polyonyma','Πολυώνυμα','Algebra','AlgPar');
INSERT INTO "Sections" VALUES ('PrakseisAr','Πράξεις αριθμών και ιδιότητες','Algebra','Arith');
INSERT INTO "Sections" VALUES ('ProsthKlasm','Πρόσθεση και αφαίρεση κλασμάτων','Algebra','Arith');
INSERT INTO "Sections" VALUES ('RhtesParast','Ρητές παραστάσεις','Algebra','AlgPar');
INSERT INTO "Sections" VALUES ('Rizes','Ρίζες πραγματικών αριθμών','Algebra','Arith');
INSERT INTO "Sections" VALUES ('SygrKlasm','Σύκριση κλασμάτων','Algebra','Arith');
INSERT INTO "Sections" VALUES ('Tautothtes','Ταυτότητες','Algebra','AlgPar');
INSERT INTO "Sections" VALUES ('TrigExisoseis','Τριγωνομετρικές εξισώσεις','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('TrigSynart','Τριγωνομετρικές συναρτήσεις','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('TrigTaut','Τριγωνομετρικές ταυτότητες','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('TrigArithmoi','Τριγωνομετρικοί αριθμοί','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('TrigArAth','Τριγωνομετρικοί αριθμοί αθροίσματος','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('TrigDiplG','Τριγωνομετρικοί αριθμοί διπλάσιας γωνίας','Algebra','Trigmt');
INSERT INTO "Sections" VALUES ('AthrGwnTr','Άθροισμα γωνιών τριγώνου - πολυγώνου','Geometria','Trig');
INSERT INTO "Sections" VALUES ('AnisTrig','Ανισοτικές σχέσεις στα τρίγωνα','Geometria','Trig');
INSERT INTO "Sections" VALUES ('AxKykloi','Αξιοσημείωτοι κύκλοι','Geometria','Kykl');
INSERT INTO "Sections" VALUES ('ApShm-Emb','Απόσταση σημείου από ευθεία - Εμβαδόν τριγώνου','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('GPythTheor','Γενικευμένο Πυθαγόρειο θεώρημα','Geometria','Trig');
INSERT INTO "Sections" VALUES ('GenExisEutheias','Γενική μορφή εξίσωσης ευθείας','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('GinArithDian','Γινόμενο αριθμού με διάνυσμα','Geometria','Dian');
INSERT INTO "Sections" VALUES ('Gwnia','Γωνία - Είδη γωνιών','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('Dianysmata','Διανύσματα','Geometria','Dian');
INSERT INTO "Sections" VALUES ('EggKanPolyg','Εγγραφή κανονικών πολυγώνων','Geometria','KanP');
INSERT INTO "Sections" VALUES ('EidhParall','Είδη παραλληλογράμμων','Geometria','Parall');
INSERT INTO "Sections" VALUES ('Elleipsi','Έλλειψη - Εξίσωση έλλειψης','Geometria','KwnT');
INSERT INTO "Sections" VALUES ('EmbadaBasSx','Εμβαδά βασικών σχημάτων','Geometria','KanP');
INSERT INTO "Sections" VALUES ('EmbadaOgkoi','Εμβαδά και όγκοι βασικών στερεών σχημάτων','Geometria','Stereom');
INSERT INTO "Sections" VALUES ('EmbKyklou','Εμβαδόν κύκλου και κυκλικού τομέα - τμήματος','Geometria','Kykl');
INSERT INTO "Sections" VALUES ('ExisEutheias','Εξίσωση ευθείας','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('EswterikoGin','Εσωτερικό γινόμενο','Geometria','Dian');
INSERT INTO "Sections" VALUES ('Eutheia','Ευθεία - Ημιευθεία - Ευθύγραμμο τμήμα','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('EuthEpip3D','Ευθείες και επίπεδα στο χώρο','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('EfekseisDiadox','Εφεξής διαδοχικές γωνίες - Άθροισμα γωνιών','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('ThDiameswn','Θεώρημα διαμέσων','Geometria','Trig');
INSERT INTO "Sections" VALUES ('ThDixotomwn','Θεώρημα διχοτόμων','Geometria','Trig');
INSERT INTO "Sections" VALUES ('ThThalh','Θεώρημα Θαλή','Geometria','Parall');
INSERT INTO "Sections" VALUES ('IsOrthTrig','Ισότητα ορθογωνίων τριγώνων','Geometria','Trig');
INSERT INTO "Sections" VALUES ('IsotTrig','Ισότητα τριγώνων','Geometria','Trig');
INSERT INTO "Sections" VALUES ('KanPolyg','Κανονικα πολύγωνα','Geometria','KanP');
INSERT INTO "Sections" VALUES ('ExisKyklou','Κύκλος - Εξίσωση κύκλου','Geometria','KwnT');
INSERT INTO "Sections" VALUES ('Kyklos','Κύκλος - Στοιχεία κύκλου','Geometria','Kykl');
INSERT INTO "Sections" VALUES ('LogosEmb','Λόγος εμβαδών','Geometria','KanP');
INSERT INTO "Sections" VALUES ('MhkosKyklou','Μήκος κύκλου και κυκλικού τόξου','Geometria','Kykl');
INSERT INTO "Sections" VALUES ('Omoiothta','Ομοιότητα','Geometria','Omoia');
INSERT INTO "Sections" VALUES ('Parabolh','Παραβολή - Εξίσωση παραβολής','Geometria','KwnT');
INSERT INTO "Sections" VALUES ('ParallEuth','Παράλληλες ευθείες','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('Parallhlogr','Παραλληλόγραμμα','Geometria','Parall');
INSERT INTO "Sections" VALUES ('ParaplSymplG','Παραπληρωματικές - Συμπληρωματικές - Κατακορυφήν γωνίες','Geometria','Eutheia');
INSERT INTO "Sections" VALUES ('ProsthDian','Πρόσθεση και αφαίρεση διανυσματων','Geometria','Dian');
INSERT INTO "Sections" VALUES ('PythTheor','Πυθαγόρειο θεώρημα - Ορθές προβολές','Geometria','Trig');
INSERT INTO "Sections" VALUES ('SyntetDian','Συντεταγμένες διανύσματος','Geometria','Dian');
INSERT INTO "Sections" VALUES ('EfaptTmhmata','Σχετική θέση ευθείας και κύκλου - Εφαπτόμενα τμήματα','Geometria','-');
INSERT INTO "Sections" VALUES ('SxetThKykl','Σχετική θέση κύκλων','Geometria','-');
INSERT INTO "Sections" VALUES ('SxhmaTGr','Τεθλασμένη γραμμή - Σχήματα - Πολύγωνα (Βασικές έννοιες)','Geometria','-');
INSERT INTO "Sections" VALUES ('TemnKyklou','Τέμνουσες κύκλου','Geometria','-');
INSERT INTO "Sections" VALUES ('Trapezio','Τραπέζιο','Geometria','-');
INSERT INTO "Sections" VALUES ('Trigwna','Τρίγωνα - Στοιχεία τριγώνου - Είδη τριγώνων','Geometria','-');
INSERT INTO "Sections" VALUES ('Yperbolh','Υπερβολή','Geometria','KwnT');
INSERT INTO "Sections" VALUES ('AntistrSyn','Αντίστροφη συνάρτηση','Analysh','Synart');
INSERT INTO "Sections" VALUES ('ArxSAorOl','Αρχική συνάρτηση - Αόριστο ολοκλήρωμα','Analysh','Olokl');
INSERT INTO "Sections" VALUES ('Asymptotes','Ασύμπτωτες','Analysh','Oria');
INSERT INTO "Sections" VALUES ('GrParastash','Γραφική παράσταση συνάρτησης','Analysh','Synart');
INSERT INTO "Sections" VALUES ('EkthetikhSyn','Εκθετική συνάρτηση','Analysh','Logar');
INSERT INTO "Sections" VALUES ('EmbXwriou','Εμβαδόν χωρίου','Analysh','Olokl');
INSERT INTO "Sections" VALUES ('ExisEfaptom','Εξίσωση εφαπτομένης γραφικής παράστασης','Analysh','Parag');
INSERT INTO "Sections" VALUES ('Synarthsh','Η έννοια της συνάρτησης','Analysh','Synart');
INSERT INTO "Sections" VALUES ('ThBolzano','Θεώρημα Bolzano','Analysh','Synex');
INSERT INTO "Sections" VALUES ('ThRolle','Θεώρημα Rolle','Analysh','Parag');
INSERT INTO "Sections" VALUES ('ThETΜΕΤ','Θεώρημα ενδιάμεσων τιμών - Θεώρημα μέγιστης και ελάχιστης τιμής','Analysh','Parag');
INSERT INTO "Sections" VALUES ('TheMTimhs','Θεώρημα μέσης τιμής','Analysh','Parag');
INSERT INTO "Sections" VALUES ('DelHospital','Κανόνας Del Hospital','Analysh','Parag');
INSERT INTO "Sections" VALUES ('Kyrtothta','Κυρτότητα - Σημεία καμπής','Analysh','Parag');
INSERT INTO "Sections" VALUES ('LogarSyn','Λογαριθμική συνάρτηση','Analysh','Logar');
INSERT INTO "Sections" VALUES ('MethOlokl','Μέθοδοι ολοκλήρωσης','Analysh','Olokl');
INSERT INTO "Sections" VALUES ('MetatGrPar','Μετατόπιση γραφικής παράστασης','Analysh','Synart');
INSERT INTO "Sections" VALUES ('MhPepOrio','Μη πεπερασμένο όριο','Analysh','Oria');
INSERT INTO "Sections" VALUES ('MonotAkrot','Μονοτονία ακρότατα συνάρτησης','Analysh','Parag');
INSERT INTO "Sections" VALUES ('OrioShmeio','Όριο σε σημείο','Analysh','Oria');
INSERT INTO "Sections" VALUES ('OrioApeiro','Όριο στο άπειρο','Analysh','Oria');
INSERT INTO "Sections" VALUES ('OrismOlokl','Ορισμένο ολοκλήρωμα','Analysh','Olokl');
INSERT INTO "Sections" VALUES ('ParagShmeio','Παράγωγος σε σημείο','Analysh','Parag');
INSERT INTO "Sections" VALUES ('Paragogos','Παράγωγος συνάρτηση','Analysh','Parag');
INSERT INTO "Sections" VALUES ('RythMetab','Ρυθμός μεταβολής','Analysh','Parag');
INSERT INTO "Sections" VALUES ('SymmGraf','Συμμετρίες γραφικής παράστασης','Analysh','Synart');
INSERT INTO "Sections" VALUES ('Synart1p1','Συνάρτηση 1-1','Analysh','Synart');
INSERT INTO "Sections" VALUES ('SynartYper','Συνάρτηση f(x)=a/x της υπερβολής','Analysh','Synart');
INSERT INTO "Sections" VALUES ('Synart2ou','Συνάρτηση f(x)=ax^2+bx+c της παραβολής','Analysh','Synart');
INSERT INTO "Sections" VALUES ('Synart1ou','Συνάρτηση f(x)=ax+b ευθεία','Analysh','Synart');
INSERT INTO "Sections" VALUES ('SynartOlokl','Συνάρτηση ολοκλήρωμα','Analysh','Olokl');
INSERT INTO "Sections" VALUES ('SynepThMT','Συνέπειες θεωρήματος μέσης τιμής','Analysh','Parag');
INSERT INTO "Sections" VALUES ('SynexeiaSyn','Συνέχεια συνάρτησης','Analysh','Synex');
INSERT INTO "Sections" VALUES ('BasikesEnStat','Βασικές έννοιες της Στατιστικής','Statistikh','-');
INSERT INTO "Sections" VALUES ('DeigmEnd','Δειγματικός χώρος - Ενδεχόμενα','Pithanothtes','Pithan');
INSERT INTO "Sections" VALUES ('Pithanothta','Η έννοια της πιθανότητας','Pithanothtes','Pithan');
INSERT INTO "Sections" VALUES ('Metyaxb','Η μεταβλητή Y=aX+b','Statistikh','-');
INSERT INTO "Sections" VALUES ('MetraDiasp','Μέτρα διασποράς','Statistikh','MetThDias');
INSERT INTO "Sections" VALUES ('MetraThesis','Μέτρα θέσης','Statistikh','MetThDias');
INSERT INTO "Sections" VALUES ('OmadParat','Ομαδοποίηση παρατηρήσεων','Statistikh','PaStD');
INSERT INTO "Sections" VALUES ('StatParDed','Παρουσίαση δεδομένων','Statistikh','-');
INSERT INTO "Sections" VALUES ('Synola','Σύνολα','Pithanothtes','Synola');
INSERT INTO "Sections" VALUES ('SynthSyn','Σύνθεση συναρτήσεων','Analysh','Synart');
INSERT INTO "Sections" VALUES ('IsesSyn','Ισότητα συναρτήσεων','Analysh','Synart');
INSERT INTO "Sections" VALUES ('BasEnnStat','Βασικές έννοιες στατιστικής','Statistikh','PaStD');
INSERT INTO "Sections" VALUES ('ParStD','Παρουσίαση στατιστικών δεδομένων','Statistikh','PaStD');
INSERT INTO "Sections_Exercises" VALUES ('1oThewr','Πρώτο θεώρημα διαμέσων','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('2oThewr','Δεύτερο θεώρημα διαμέσων','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('ARrAn2Riz','Άρρητες ανισώσεις με δύο ρίζες','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('AfairAri','Αφαίρεση αριθμών','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('AkAnadTyp','Αναδρομικός τύπος ακολουθίας','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('AkGenTyp','Γενικός τύπος ακολουθίας','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('AkrKlDiast','Ακρότατα με τη χρήση μονοτονίας σε κλειστό διάστημα','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('AkrotAnis','Εύρεση ακροτάτων με βασικές ανισότητες','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('AkrotAnis','Εύρεση ακροτάτων με βασικές ανισότητες','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('AkrotPar','Εύρεση ακροτάτων με τη χρήση παραγώγου','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('AkrotPar','Εύρεση ακροτάτων με τη χρήση παραγώγου','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('AktDiam','Ακτίνα - Διάμετρος - Κέντρο κύκλου','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('AlgParRiz','Πεδίο ορισμού άρρητης αλγεβρικής παράστασης','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('AllagBash','Αλλαγή βάσης λογαρίθμου','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('An2Trig','Ανισότητα πλευρών - γωνιών σε δύο τρίγωνα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('AnHmx','Η ανισότητα |ημx|<=x','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('AnPlGwn','Ανισότητα πλευρών και γωνιών τριγώνου','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('AnTypGenT','Μετατροπή αναδρομικού τύπου σε γενικό και αντίστροφα','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('AnaPerVenn','Μετατροπή συνόλου ανάμεσα σε αναγραφή - περιγραφή και διάγραμμα Venn','Synola');
INSERT INTO "Sections_Exercises" VALUES ('Anag1oTet','Αναγωγή στο 1ο τεταρτημόριο','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('AnagTyp','Αναγωγικοί τύποι','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Anagrafh','Αναγραφή συνόλου','Synola');
INSERT INTO "Sections_Exercises" VALUES ('AnakIdiot','Ανακλαστική ιδιότητα','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('AnakIdiot','Ανακλαστική ιδιότητα','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('AnakIdiot','Ανακλαστική ιδιότητα','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('AnakIdiot','Ανακλαστική ιδιότητα','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('AnalPrwt','Ανάλυση αριθμού σε γινόμενο πρώτων παραγόντων','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('AnapKybou','Ανάπτυγμα κύβου','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('AnapTetr','Ανάπτυγμα τετραγώνου','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('AnisAnt','Επίλυση ανίσωσης με αντικατάσταση','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('AnisAnt','Επίλυση ανίσωσης με αντικατάσταση','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('AnisAnt','Επίλυση ανίσωσης με αντικατάσταση','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('AnisApT','Ανισώσεις με απόλυτες τιμές','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('AnisGin','Ανισώσεις γινόμενο - Πρόσημο Πολυωνύμου','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('AnisHmSyn','Ανισοτικές σχέσεις ημιτόνου και συνημιτόνου','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('AntPythT','Αντίστροφο Πυθαγορείου θεωρήματος','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Synola');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Antipar','Αντιπαράδειγμα','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Synola');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Antistoix','Αντιστοίχιση','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('AntistrAr','Αντίστροφοι αριθμοί','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('AntistrThal','Αντίστροφο θεωρήματος Θαλή','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('AntistrThal','Αντίστροφο θεωρήματος Θαλή','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('Antistyx','Οι εξισώσεις f^(-1)(x)=x - f^(-1)(x)=f(x)','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('AntithAr','Αντίθετοι αριθμοί','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('AorAdynAn','Αόριστες και αδύνατες ανισώσεις 1ου βαθμού','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('AorAdynEx','Αόριστες και αδύνατες εξισώσεις 1ου βαθμού','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Ap1p1Mon','Απόδειξη συνάρτησης 1 - 1 με χρήση μονοτονίας','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('Ap1p1Or','Απόδειξη συνάρτησης 1 - 1 με χρήση ορισμού','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('ApAkrAnis','Απόδειξη ακρότατου με τη βοήθεια ανισότητας','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('ApEuth','Απόσταση μεταξύ παράλληλων ευθειών','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('ApEuth','Απόσταση μεταξύ παράλληλων ευθειών','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('ApShmEpip','Απόσταση σημείου επιπέδου - παράλληλων επιπέδων','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('ApTimAnis','Υπολογισμός παράστασης με απόλυτα με δοσμένη ανισότητα','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('ApTimId','Ιδιότητες απόλυτων τιμών','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('ApTimPer','Υπολογισμός παράστασης με απόλυτα με περιπτώσεις','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('ApagAtopo','Απαγωγή σε άτοπο','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('AplTrExis','Απλές τριγωνομετρικές εξισώσεις','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('AplopKl','Απλοποίηση κλάσματος - Ανάγωγα κλάσματα','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('AplopRht','Απλοποίηση ρητής παράστασης','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('AplopRht','Απλοποίηση ρητής παράστασης','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('AplopRiz','Απλοποίηση ρίζας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Synola');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('ApoTheorias','Αποδείξεις θεωρίας','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('ApodAnis','Απόδειξη ανισότητας','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('ApodAnis','Απόδειξη ανισότητας','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('ApodAnis','Απόδειξη ανισότητας','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('ApodAnis','Απόδειξη ανισότητας','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ApodAnis','Απόδειξη ανισότητας','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('ApodAnis','Απόδειξη ανισότητας','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('ApodEfapt','Συνθήκες για να εφάπτεται μια ευθεία','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('ApodIsot','Απόδειξη ταυτότητας - ισότητας','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('ApoklDiaz','Αποκλειστική διάζευξη','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('AposAnisD','Μετατροπή μεταξύ απόστασης αριθμών - ανισότητας με απόλυτα και διαστήματα','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('AposAri','Απόσταση αριθμών','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('ApostShm','Απόσταση σημείων','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('ApostShmE','Απόσταση σημείου από ευθεία','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('ApostShmE','Απόσταση σημείου από ευθεία','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('AposynSyn','Αποσύνθεση συναρτήσεων','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Apotetr','Τύποι αποτετραγωνισμού γωνίας','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('ArPrOrAth','Προσδιορισμός όρων α.π. από άθροισμα','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('ArithMes','Αριθμητικός μέσος - Διαδοχικοί όροι αριθμητικής προόδου','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('ArithPar','Αριθμητική παρεμβολή','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('Arnhsh','Άρνηση','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('ArrAn1Riz','Άρρητες ανισώσεις με μια ρίζα','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('ArrEx1Riz','Άρρητες εξισώσεις με μια ρίζα','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('ArrEx2Riz','Άρρητες εξισώσεις με δύο ρίζες','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('ArtioiPer','Άρτιοι - Περιττοί αριθμοί','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('ArtiosEkth','Πρόσημο δύναμης με άρτιο εκθέτη','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('ArxBasSyn','Αρχική βασικών συναρτήσεων','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('ArxGinPil','Αρχική με κανόνα γινομένου ή πηλίκου','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('ArxOria','Αρχική και όρια','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('ArxPollTyp','Αρχική συνάρτησης πολλαπλού τύπου - απόλυτης τιμής','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('ArxSynth','Αρχική σύνθετων συναρτήσεων','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('ArxThewr','Αρχική συνάρτηση και θεωρήματα','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('AsymOria','Ασύμπτωτες και όρια','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('AsymptYp','Ασύμπτωτες υπερβολής','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('AsymptYp','Ασύμπτωτες υπερβολής','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('AthArPr','Άθροισμα ν πρώτων όρων αριθμητικής προόδου','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('AthDiafK','Άθροισμα - Διαφορά κύβων','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('AthGwn','Άθροισμα γωνιών','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('AthGwnPol','Άθροισμα γωνιών ν-γωνου','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('AthGwnTr','Αθροισμα γωνιών τριγώνου','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('AthrGeoPr','Άθροισμα ν πρώτων όρων γεωμετρικής προόδου','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('AxiaPsif','Αξία ψηφίου','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('AxiomXwr','Αξιώματα χώρου','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('BSynVen','Βασικό σύνολο - Διάγραμμα Venn','Synola');
INSERT INTO "Sections_Exercises" VALUES ('BarTrig','Βαρύκεντρο τριγώνου','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('BarTrig','Βαρύκεντρο τριγώνου','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('Baryken','Βαρύκεντρο - ορθόκεντρο τριγώνου','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('BasEnnDian','Βασικές έννοιες διανυσμάτων','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('BasEnnSt','Βασικές έννοιες στατιστικής','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('BasGrPar','Γραφικές παραστάσεις βασικών συναρτήσεων','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('BasGrPar','Γραφικές παραστάσεις βασικών συναρτήσεων','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('BasTaut','Βασικές πολυωνυμικές ταυτότητες','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('BathMon','Βαθμός μονωνύμου','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('BathmPol','Βαθμός πολυωνύμου','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('BohthSyn','Βοηθητική συνάρτηση','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('BohthSyn','Βοηθητική συνάρτηση','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('BohthSyn','Βοηθητική συνάρτηση','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('BohthSyn','Βοηθητική συνάρτηση','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('BolzLim','Θ. Bolzano με χρήση ορίων','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('DekKlasm','Μετατροπή μεταξύ δεκαδικού αριθμού - κλάσματος','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('DekKlasm','Μετατροπή μεταξύ δεκαδικού αριθμού - κλάσματος','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('DekadAr','Η έννοια του δεκαδικού αριθμού','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('DeutStoix','Δευτερεύοντα στοιχεία τριγώνου','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Synola');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('DiLathous','Διόρθωση λάθους','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('DiadGwn','Διαδοχικές γωνίες','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('DiadRolle','Διαδοχικές εφαρμογές του Θ. Rolle','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('DiadSygkr','Διαδοχικές συγκρίσεις τριγώνων','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('DiafTetr','Διαφορά τετραγώνων','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('Diagramma','Διάγραμμα - Πολύγωνο συχνοτήτων','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('DiairAkKl','Διαίρεση ανάμεσα σε ακέραιο και κλάσμα','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('DiairArith','Διαίρεση - Ευκλείδεια διαίρεση αριθμών','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('DiairKl','Διαίρεση κλασμάτων','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('DiairPol','Διαίρεση πολυωνύμων - Αλγόριθμος','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('Diairetes','Διαιρέτες ακέραιου αριθμού','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Diakym','Διακύμανση','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('Diamesos','Διάμεσος από παρατηρήσεις','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('DianAktMes','Διανυσματική ακτίνα μέσου τμήματος','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('DianIsot','Διανυσματικές ισότητες - Απόδειξη ιδιοτήτων','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('DianIsot','Διανυσματικές ισότητες - Απόδειξη ιδιοτήτων','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('DianThesis','Διάνυσμα θέσης - Διαφορά διανυσματικών ακτίνων','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('DiaspKlasm','Διάσπαση κλάσματος σε άθροισμα','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('Diasthmata','Διαστήματα','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('DiaxXwr','Διαχωρισμός χωρίου','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('Diazeuxh','Διάζευξη','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('DiplAnis','Διπλές ανισότητες - ανισώσεις','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('DiplAr','Τύπος διπλασιασμού του Αρχιμήδη','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('DixotGwn','Διχοτόμος γωνίας','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Dixotomos','Διχοτόμος γωνίας','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('DynArnBas','Δυνάμεις με αρνητική βάση - Αντίθετες βάσεις','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('DynRhtEk','Δυνάμεις με ρητό εκθέτη','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('Dynam10','Δυνάμεις του 10 - Τυποποιημένη μορφή','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('DynamShm','Δύναμη σημείου ως προς κύκλο','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EKPAr','Ε.Κ.Π. αριθμών','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('EKPMon','Ε.Κ.Π. - Μ.Κ.Δ. μονωνύμων','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('EPKPol','Ε.Κ.Π. - Μ.Κ.Δ. πολυωνύμων','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('EfExShm','Εφαπτομένη που διέρχεται από γνωστό σημείο','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('EfGnKlish','Εφαπτομένη με γνωστή κλίση','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('EfGnSEp','Εφαπτομένη με γνωστό σημείο επαφής','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('EfParOTr','Εφαρμογές παραλληλογράμμων στα ορθογώνια τρ.','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('EfRolle','Εφαρμογή Θ. Rolle','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('EfThMET','Εφαρμογή θεωρήματος μέγιστης - ελάχιστης τιμής','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('EfThMT','Εφαρομγή Θ.Μ.Τ.','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('EfThalTr','Εφαρμογή του θ. Θαλή στα τρίγωνα','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('EfapAnis','Εφαπτομένες και ανισότητες','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('EfapOria','Εφαπτομένες και όρια','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('EfapTmhm','Εφαπτόμενα τμήματα','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('EfexhsGwn','Εφεξής γωνίες','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('Egg81012','Εγγραφή κανονικού οκταγώνου - δεκαγώνου - δωδεκαγώνου','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('EggBasP','Εγγραφή βασικών κανονικών πολυγώνων σε κύκλο','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('EggKykl','Εγγεγραμμένος κύκλος','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('EggTetr','Εγγεγραμμένο - εγγράψιμο τετράπλευρο','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EidMetD','Είδος μεταβλητής - Αντιπροσωπευτικό δείγμα','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('EidhGwn','Είδη γωνιών','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('EidhRizwn','Είδη και πλήθος ριζών τριωνύμου 2ου β.','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('EidhRizwn','Είδη και πλήθος ριζών τριωνύμου 2ου β.','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('EidhTrig','Είδη τριγώνων (ως προς πλευρές και γωνίες)','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('EidosGwn','Είδος γωνίας τριγώνου','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('EkfParKl','Έκφραση παράστασης σε κλάσμα','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('Ekkentr','Εκκεντρότητα','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('Ekkentr','Εκκεντρότητα','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('EkthAnis','Εκθετικές ανισώσεις','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('EkthExis','Εκθετικές εξισώσεις','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('EkthSyst','Εκθετικά συστήματα','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('EllPinakas','Συμπλήρωση ελλειπή πίνακα','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('EllPinakas','Συμπλήρωση ελλειπή πίνακα','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('EmbAntist','Εμβαδόν και αντίστροφή συνάρτηση','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('EmbCfCg','Εμβαδόν μεταξύ δύο γραφικών παραστάσεων','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('EmbCfEf','Εμβαδόν μεταξύ γρ. παράστασης και εφαπτομένης','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('EmbCfxx','Εμβαδόν μεταξύ γρ. παράστασης και άξονα xx','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('EmbDiam','Εμβαδόν και διάμεσος τριγώνου','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EmbKykTm','Εμβαδόν κυκλικού τμήματος','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EmbKykTom','Εμβαδόν κυκλικού τομέα','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EmbKykl','Εμβαδόν κύκλου','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EmbMeikt','Εμβαδόν μεικτόγραμμου σχήματος','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EmbMhn','Εμβαδόν μηνίσκου','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EmbOrth','Εμβαδόν ορθογωνίου','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EmbPar','Εμβαδόν παραλληλογράμμου','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EmbPolEp','Εμβαδόν πολυγωνικής επιφάνειας','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EmbRomb','Εμβαδόν ρόμβου - τετραπλ. με κάθετες διαγ.','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EmbTetr','Εμβαδόν τετραγώνου','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EmbTrGP','Εμβαδόν μεταξύ τριών γραφικών παραστάσεων','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('EmbTrap','Εμβαδόν τραπεζίου','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EmbTrig','Εμβαδόν τριγώνου','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('EmbTrig','Εμβαδόν τριγώνου','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EnSynol','Έννοια συνόλου - Ανήκει - Δεν ανήκει','Synola');
INSERT INTO "Sections_Exercises" VALUES ('EpMGSTex','Επίλυση μη γραμμικού συστήματος με τεχνάσματα','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('EpalIsot','Επαλήθευση ισότητας - λύση εξίσωσης','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Epil2ouEP','Επίλυση εξίσωσης 2ου β. όταν β=0 ή γ=0','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('EpilAnis','Επίλυση απλής πολυωνυμικής ανίσωσης','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('EpilAnis','Επίλυση απλής πολυωνυμικής ανίσωσης','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('EpilAnisS','Επίλυση ανισώσεων','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('EpilAnisS','Επίλυση ανισώσεων','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('EpilAnisS','Επίλυση ανισώσεων','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('EpilAnisS','Επίλυση ανισώσεων','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('EpilExis','Επίλυση απλής πολυωνυμικής εξίσωσης','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('EpilExis','Επίλυση απλής πολυωνυμικής εξίσωσης','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('EpilExisS','Επίλυση εξισώσεων','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('EpilExisS','Επίλυση εξισώσεων','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('EpilExisS','Επίλυση εξισώσεων','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('EpilTrig','Επίλυση τριγώνου','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('EpilTyp','Επίλυση τύπου','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('EpipHmi','Επίπεδο - Ημιεπίπεδο','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Synola');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('EplTheor','Επιλογή θεωρίας','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Synola');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('ErwTheor','Ερωτήσεις θεωρίας','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('EswtGinom','Εσωτερικό γινόμενο διανυσμάτων','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('EuAntPollT','Εύρεση αντίστροφης συνάρτησης πολλαπλού τύπου','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('EuParMes','Ευθεία παράλληλη από μέσο πλευράς','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('EurAntistr','Εύρεση αντίστροφης συνάρτησης','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('EurTrigAr','Εύρεση τριγωνομετρικών αριθμών','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('EurTrigAr','Εύρεση τριγωνομετρικών αριθμών','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('EurTypou','Εύρεση τύπου συνάρτησης','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('EurTypou','Εύρεση τύπου συνάρτησης','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('EurTypou','Εύρεση τύπου συνάρτησης','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('EuthApod','Ευθεία απόδειξη','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('EuthArith','Ευθεία των αριθμών','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('EuthArith','Ευθεία των αριθμών','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('EuthIdiot','Ευθεία με γνωστό σημείο - συντελεστή και ιδιότητα','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('EuthIdiot','Ευθεία με γνωστό σημείο - συντελεστή και ιδιότητα','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('EuthKinish','Ευθύγραμμη κίνηση','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('EuthTm','Ευθύγραμμο τμήμα - Μέσο τμήματος','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('Eutheia','Ευθεία γραμμή','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('ExEfKyk','Εξίσωση εφαπτομένης κύκλου (αρχή αξόνων + τυχαία θέση)','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ExEfapt','Εξίσωση εφαπτομένης','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('ExEfapt','Εξίσωση εφαπτομένης','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('ExEfapt','Εξίσωση εφαπτομένης','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('ExEuthShm','Εξίσωση ευθείας με γνωστό σημείο (επί ή εκτός)','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ExEuthSynt','Εξίσωση ευθείας με γνωστό συντελεστή','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ExEuthSynt','Εξίσωση ευθείας με γνωστό συντελεστή','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('ExGwn','Σχέση εξωτερικής γωνίας με απέναντι γωνίες τριγώνου','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('ExKyklou','Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('ExisAnt','Επίλυση εξίσωσης με αντικατάσταση','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('ExisApolTim','Εξισώσεις με απόλυτες τιμές','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('ExisXnA','Εξισώσεις x^n=a','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('ExisXnAn','Εξισώσεις x^n=a^n','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('ExwtGwn','Εξωτερική γωνία τριγώνου','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Fapof2','Εύρεση συνάρτησης f από f^2','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('GPAmbl','Γενικευμένο Πυθαγόρειο θεώρημα για αμβλεία γωνία','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('GPOxeia','Γενικευμένο Πυθαγόρειο θεώρημα για οξεία γωνία','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('GePrOrGin','Προσδιορισμός όρων γ.π. από γινόμενο','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('GenExKykl','Γενική εξίσωση κύκλου','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('GenOrArPr','Γενικός όρος αριθμητικής προόδου - Εύρεση όρων και βήματος ω','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('GenOrGePr','Γενικός όρος γεωμετρικής προόδου - Εύρεση όρων και λόγου λ','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('GeomEfarm','Γεωμετρικές εφαρμογές','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('GeomMes','Γεωμετρικός μέσος - Διαδοχικοί όροι γεωμετρικής προόδου','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('GeomParem','Γεωμετρική παρεμβολή','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('GeomTopoi','Γεωμετρικοί τόποι','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('GinomArith','Γινόμενο αριθμών','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('GrParAnt','Γραφικές παραστάσεις των -f(x) - f(-x) - |f(x)|','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('GrParDed','Γραφική παράσταση δεδομένων','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('GrafEpil','Γραφική επίλυση συστήματος','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('GramExis','Γραμμική εξίσωση - Λύση -  Σημείο - Τομή με άξονες - Σχεδίαση','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('GrammSyn','Γραμμικός συνδυασμός διανυσμάτων','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('GwnPEuth','Από γωνίες σε παράλληλες ευθείες','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('GwnTrig','Γωνίες τριγώνου - πολυγώνου (προσκείμενες-περιεχόμενες)','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('GwnTrig','Γωνίες τριγώνου - πολυγώνου (προσκείμενες-περιεχόμενες)','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Gwnia','Γωνία - Ορισμός γωνίας','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('GwniaDian','Γωνία διανυσμάτων','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('GwniaEuth','Γωνία ευθειών','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('GwniaEuth','Γωνία ευθειών','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Hmieuth','Ημιευθεία','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('IdTetRiz','Ιδιότητες τετραγωνικής ρίζας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('IdiotAnis','Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('IdiotDynam','Ιδιότητες δυνάμεων','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('IdiotEsGin','Ιδιότητες εσωτερικού γινομένου','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('IdiotGin','Ιδιότητες γινομένου αριθμού με διάνυσμα','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('IdiotIsot','Ιδιότητες ισοτήτων','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('IdiotLog','Ιδιότητες λογαρίθμων','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('IdiotNRiz','Ιδιότητες ν-οστής ρίζας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('IdiotOlokl','Ιδιότητες ορισμένου ολοκληρώματος','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('IdiotOria','Ιδιότητες ορίων','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('IdiotOria','Ιδιότητες ορίων','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('IdiotOria','Ιδιότητες ορίων','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('IdiotSymm','Ιδιότητες άρτιων και περιττών συναρτήσεων','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('IdiotSyxn','Ιδιότητες συχνοτήτων','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('IdiotSyxn','Ιδιότητες συχνοτήτων','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('IsaAntDian','Ίσα - Αντίθετα διανύσματα','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('IsaMhdPar','Ίσα διανύσματα - Μηδενικό δ. - Παράλληλα με άξονες','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('IsaSynol','Ίσα σύνολα','Synola');
INSERT INTO "Sections_Exercises" VALUES ('IsesPar','Συναρτήσεις με ίσες παραγώγους σε διάστημα','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('IsesParD','Συναρτήσεις με ίσες παραγώγους σε ένωση διαστημάτων','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('IsesSynart','Ισότητα συναρτήσεων','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('IsesSynart','Ισότητα συναρτήσεων','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('IsodKlasm','Ισοδύναμα κλάσματα','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Isodynamia','Ισοδυναμία','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('IsosTrap','Ισοσκελές τραπέζιο','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('IsoskTrig','Ισοσκελή τρίγωνα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('IsoskTrig','Ισοσκελή τρίγωνα','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('IsoskTrig','Ισοσκελή τρίγωνα','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('IsoskTrig','Ισοσκελή τρίγωνα','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('IsotPolyon','Ισότητα πολυωνύμων','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('IstogrSyxn','Ιστόγραμμα και πολύγωνο συχνοτήτων','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('KanDiair','Κανόνες διαιρετότητας','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('KanonKat','Κανονική κατανομή','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('KatPinaka','Κατασκευή πίνακα - Επεξεργασία δεδομένων','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('KatPinaka','Κατασκευή πίνακα - Επεξεργασία δεδομένων','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('KatakAsym','Κατακόρυφη ασύμπτωτη','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('KatakMet','Κατακόρυφη μετατόπιση γραφικής παράστασης','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('KatakMet','Κατακόρυφη μετατόπιση γραφικής παράστασης','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('KatakorGwn','Κατακορυφήν γωνίες','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Synola');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('KataskAskhshs','Κατασκευή άσκησης - προβλήματος','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('KathDian','Κάθετα διανύσματα','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('KathPlTm','Κάθετα και πλάγια τμήματα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('Kathetes','Κάθετες ευθείες','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Kathetes','Κάθετες ευθείες','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('KenoSyn','Κενό σύνολο','Synola');
INSERT INTO "Sections_Exercises" VALUES ('KinisKamp','Κίνηση σε καμπύλη','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('KlaEx1ou','Εξισώσεις που ανάγονται σε 1ου βαθμού','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('KlaEx2ou','Εξισώσεις που ανάγονται σε 2ου βαθμού','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('KlaEx3ou','Εξισώσεις που ανάγονται σε 3ου βαθμού','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('KlasAnis','Κλασματικές ανισώσεις','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('KoinEf','Κοινές εφαπτομένες κύκλων','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('KoinEfDS','Κοινή εφαπτομένη σε διαφορετικά σημεία','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('KoinEfKS','Κοινή εφαπτομένη σε κοινό σημείο','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('KoinLys','Κοινές λύσεις ανισώσεων και διπλών ανισώσεων','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('KoinLys','Κοινές λύσεις ανισώσεων και διπλών ανισώσεων','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('KoinPar','Κοινός παράγοντας','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('KoinesEfap','Κοινές εφαπτομένες','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('KorParab','Κορυφή - άξονας παραβολής','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('KritIsot','Κριτήρια ισότητας τριγώνων','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('KritIsot','Κριτήρια ισότητας τριγώνων','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('KritParem','Κριτήριο παρεμβολής','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('KritParem','Κριτήριο παρεμβολής','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('KritParem','Κριτήριο παρεμβολής','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('KritParem','Κριτήριο παρεμβολής','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('Kwnos','Κώνος','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('KybParEp','Κύβος - Παραλληλεπίπεδο - Πρίσμα','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('KyklDiagr','Κυκλικό διάγραμμα συχνοτήτων','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('Kylindr','Κύλινδρος','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('KyrStoix','Κύρια στοιχεία τριγώνου','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('KyrtEfapt','Κυρτότητα και εφαπτομένες','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('KyrtPollT','Κυρτότητα - σημεία καμπής συν. πολλαπλού τύπου - απόλυτης τιμής','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('LogEmIs','Λόγος εμβαδών τριγώνων με ίσες βάσεις - ύψη','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('LogGwn','Λόγος εμβαδών τριγώνων με ίσες ή παραπλ. γωνίες','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('LogOmP','Λόγος εμβαδών ομοίων τριγώνων - πολυγώνων','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('LogarAnis','Λογαριθμικές ανισώσεις','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('LogarExis','Λογαριθμικές εξισώσεις','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('LogarSyst','Λογαριθμικά συστήματα','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('MDOmad','Μέτρα διασποράς από ομαδοποιημένα δεδομένα','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('MDParat','Μέτρα διασποράς από παρατηρήσεις','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('MDSynx','Μέτρα διασποράς από πίνακς συχνοτήτων','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('MKDAr','Μ.Κ.Δ. αριθμών','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('MParEnD','Μηδενική παράγωγος σε ένωση διαστημάτων','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('MTDiamOm','Μέση τιμή και διάμεσος από ομαδοποιημένα δεδομένα','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('MTDiamSyx','Μέση τιμή και διάμεσος από πίνακα συχνοτήτων','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('MeikKl','Μετατροπή μεταξύ μεικτού και κλάσματος','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('MelKyrtot','Μελέτη συνάρτησης ως προς την κυρτότητα','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('MelKyrtot','Μελέτη συνάρτησης ως προς την κυρτότητα','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('MelSynSym','Μελέτη συνάρτησης ως προς συμμετρία','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('MelSynSym','Μελέτη συνάρτησης ως προς συμμετρία','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('MelethSyn','Μελέτη συνάρτησης','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('MelethSyn','Μελέτη συνάρτησης','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('MelethSyn','Μελέτη συνάρτησης','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('MelethSyn','Μελέτη συνάρτησης','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('MelethSyn','Μελέτη συνάρτησης','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('MelethSyn','Μελέτη συνάρτησης','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('MelethSyn','Μελέτη συνάρτησης','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('MesTimh','Μέση τιμή από παρατηρήσεις','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('Mesokath','Μεσοκάτθετος τμήματος','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('MetIsod','Μετατροπή ισοδύναμου κλάσματος','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('MethAnt','Μέθοδος αντικατάστασης','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('MethAnt','Μέθοδος αντικατάστασης','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('MethAntS','Μέθοδος αντίθετων συντελεστών','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('MethOriz','Μέθοδος οριζουσών','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('MetrGrSyn','Μέτρο γραμμικού συνδυασμού','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('MetrGwn','Μέτρο - σύγκριση γωνιών','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('MetroDian','Μέτρο διανύσματος','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Metyax','Η μεταβλητή y=ax+b - Μεταβολές παρατηρήσεων','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('MhdFragm','Όριο μηδενικής επί φραγμένης','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('MhdOlok','Μηδενικό ολοκλήρωμα','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('MhkKenAkt','Μήκος - κέντρο και ακτίνα διαστήματος','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('MhkKykl','Μήκος κύκλου','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('MhkTox','Μήκος τόξου','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('MhkosTm','Μήκος - σύγκριση ευθυγράμμων τμημάτων','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('MidAthr','Μηδενικό άθροισμα','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('MidAthr','Μηδενικό άθροισμα','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('MidAthr','Μηδενικό άθροισμα','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('MidGinom','Μηδενικό και μη μηδενικό γινόμενο','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('MoirAkt','Μετατροπή μοιρών σε ακτίνια και αντίστροφα','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('MonAkrGrPar','Μονοτονία και ακρότατα από γραφική παράσταση','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('MonAnt','Μονοτονία αντίστροφης συνάρτησης','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('MonPolPar','Μονοτονία με χρήση 2ης - 3ης παραγώγου - βοηθητικής','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('MonPollT','Μονοτονία ακρότητα συν. πολλαπλού τύπου - Απόλυτη τιμή','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('MonadRiz','Μοναδική ρίζα','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('MonotPar','Μελέτη μονοτονίας με τη χρήση παραγώγου','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('MonotPar','Μελέτη μονοτονίας με τη χρήση παραγώγου','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Monoton','Μελέτη μονοτονίας συνάρτησης με ορισμό','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('Monoton','Μελέτη μονοτονίας συνάρτησης με ορισμό','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('NRizes','Ύπαρξη ν ριζών','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('NomHmit','Νόμος ημιτόνων','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('NomHmit','Νόμος ημιτόνων','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('NomosSyn','Νόμος συνημιτόνων','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('NomosSyn','Νόμος συνημιτόνων','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('OikEutheiwn','Οικογένεια ευθειών','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('OlokAnis','Ορισμένο ολοκλήρωμα και ανισότητες','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokAntik','Ολοκλήρωση με αντικατάσταση','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokAntist','Ολοκλήρωμα αντίστροφης συνάρτησης','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokArPer','Ολοκλήρωμα άρτιας - περιττής και περιοδικής','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokOria','Ολοκλήρωμα και όρια','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokOria','Ολοκλήρωμα και όρια','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokParag','Παραγοντική ολοκλήρωση','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokPollT','Ολοκλήρωμα συνάρτησης πολλαπλού τύπου - απόλυτης τιμής','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokRhtwn','Ολοκληρώματα ρητών συναρτήσεων','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OlokSynd','Ολοκλήρωμα μέσα σε ολοκλήρωμα','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('OmDeutSt','Όμοια τρίγωνα και δευτερεύοντα στοιχεία','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('OmEterAr','Ομόσημοι και ετερόσημοι αριθμοί','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('OmEterKl','Ομόνυμα - ετερώνυμα κλάσματα','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('OmIsaAntM','Όμοια - Ίσα - Αντίθετα μονώνυμα','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('OmKanPol','Κανονικά πολύγωνα και ομοιότητα','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('OmSxhm','Όμοια σχήματα','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Omadop','Ομαδοποίηση','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('OmoiaTr','Κριτήρια ομοιότητας τριγώνων','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Or0epiAp','Απροσδιοριστία 0 επί άπειρο','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Or0p0AppAp','Απροσδιοριστία 0 προς 0 ή άπειρο προς άπειρο','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Or0p0PTAp','Απροσδιοριστία 0 προς 0 πολλαπλού τύπου - απόλυτες τιμές','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Or0p0Riz','Απροσδιοριστία 0 προς 0 άρρητης','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Or1p0ap0','Όρια της μορφής 1 προς 0 και α προς 0','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('Or1p1','Ορισμός συνάρτησης 1 - 1','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('OrApTim','Όριο με απόλυτες τιμές','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('OrApTim','Όριο με απόλυτες τιμές','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('OrApolTim','Ορισμός απόλυτης τιμής','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('OrAprEkth','Απροσδιοριστία εκθετικής μορφής','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('OrArrAp','Όριο άρρητης συνάρτησης στο άπειρο','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('OrEkthAp','Όριο εκθετικής συνάρτησης','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('OrGrPar','Ορισμός γρ. Παράστασης - Σημείο που ανήκει στη γρ. παρ','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('OrKatMet','Συνδυασμός μετατοπίσεων','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('OrKatMet','Συνδυασμός μετατοπίσεων','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('OrLogAp','Όριο λογαριθμικής συνάρτησης στο άπειρο','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('OrPolyon','Όριο πολυωνυμικής - ρητής συνάρτησης','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('Orapmap','Απροσδιοριστία άπειρο - άπειρο','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Orapmap','Απροσδιοριστία άπειρο - άπειρο','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('OriaPraks','Όρια και πράξεις - Υπολογισμός','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Orio0p0Pol','Απροσδιοριστία 0 προς 0 ρητής','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('OrioAnis','Υπολογισμός ορίου από ανισότητα','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('OrioAnis','Υπολογισμός ορίου από ανισότητα','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('OrioGrPar','Όριο συνάρτησης από γραφική παράσταση','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('OrioGrPar','Όριο συνάρτησης από γραφική παράσταση','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('OrioSynth','Όριο σύνθετης - Μέθοδος αλλαγής μεταβλητής','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('OrisExis','Ορισμός εξίσωσης','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('OrisExis','Ορισμός εξίσωσης','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('OrisExis','Ορισμός εξίσωσης','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('OrisRMet','Ορισμός ρυθμού μεταβολής','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('OrisRizas','Ορισμός ρίζας','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('OrisSyn','Ορισμός συνάρτησης','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('OrismAkol','Ορισμός ακολουθίας','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('OrismArPr','Ορισμός αριθμητικής προόδου','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('OrismGePr','Ορισμός γεωμετρικής προόδου','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('OrismMon','Ορισμός μονωνύμου','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('OrismPol','Ορισμός πολυωνύμου','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('OrizAsym','Οριζόντια ασύμπτωτη','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('OrizMet','Οριζόντια μετατόπιση γραφικής παράστασης','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('OrizMet','Οριζόντια μετατόπιση γραφικής παράστασης','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('OrthAmb','Ορθογώνια και αμβλυγώνια τρίγωνα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('OrthProb','Θεώρημα προβολών','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Orthog','Ορθογώνιο','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('PEuGwn','Από παράλληλες ευθείες σε γωνίες','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('POrRhtP','Πεδίο ορισμού ρητής παράστασης','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('ParAnTax','Παράγωγοι ανώτερης τάξης','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParAnis','Υπολογισμός παραγώγου από ανισότητα','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ParAntistr','Παράγωγος αντίστροφης συνάρτησης','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParArtPer','Παράγωγος άριας και περιττής συνάρτησης','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParEuIsT','Παράλληλες ευθείες που ορίζουν ίσα τμήματα','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('ParIsSyn','Ισότητα της μορφής f(x)=f(x)','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('ParKathDian','Παράλληλο ή κάθετο διάνυσμα σε ευθεία','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ParPollTyp','Παράγωγος συνάρτησης πολλαπλού τύπου - απόλυτες τιμές','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ParPollTyp','Παράγωγος συνάρτησης πολλαπλού τύπου - απόλυτες τιμές','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParSynEkth','Παράγωγος σύνθετης εκθετικής','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParSynex','Παράγωγος και συνέχεια','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ParSynthRiz','Παράγωγος σύνθετης ν-οστής ρίζας','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParagAth','Παράγωγος αθροίσματος','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParagEx','Επίλυση εξίσωσης με παραγοντοποίηση','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('ParagEx','Επίλυση εξίσωσης με παραγοντοποίηση','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('ParagEx','Επίλυση εξίσωσης με παραγοντοποίηση','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ParagEx','Επίλυση εξίσωσης με παραγοντοποίηση','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('ParagEx','Επίλυση εξίσωσης με παραγοντοποίηση','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('ParagEx','Επίλυση εξίσωσης με παραγοντοποίηση','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('ParagGin','Παράγωγος γινομένου','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParagOlok','Παράγωγος','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('ParagOria','Παράγωγοι και όρια','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ParagOria','Παράγωγοι και όρια','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParagPilik','Παράγωγος πηλίκου','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParagShm','Παράγωγος συνάρτησης σε σημείο','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('ParagSynth','Παράγωγος σύνθετης συνάρτησης (Πίνακας)','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('ParagTriwn','Παραγοντοποίηση τριωνύμου','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ParagTriwn','Παραγοντοποίηση τριωνύμου','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('ParallKath','Συνθήκη παραλληλίας - καθετότητας ευθειών','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ParallTemn','Παράλληλες - Τεμνόμενες ευθείες','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('ParallTemn','Παράλληλες - Τεμνόμενες ευθείες','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('ParallTemn','Παράλληλες - Τεμνόμενες ευθείες','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('Parallhl','Παραλληλόγραμμο','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Synola');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Parametrikes','Παραμετρικές','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('ParaplGwn','Παραπληρωματικές γωνίες','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('ParegKykl','Παρεγγεγραμμένος κύκλος','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('PedioOrism','Πεδίο ορισμού συνάρτησης','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('PedioOrism','Πεδίο ορισμού συνάρτησης','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('PedioOrism','Πεδίο ορισμού συνάρτησης','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('PedioOrism','Πεδίο ορισμού συνάρτησης','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('PedioOrism','Πεδίο ορισμού συνάρτησης','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('PerigKykl','Περιγεγραμμένος κύκλος','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('PerigrSyn','Περιγραφή συνόλου','Synola');
INSERT INTO "Sections_Exercises" VALUES ('PeriodSyn','Έλεγχος περιοδικότητας - Εύρεση περιόδου','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Synola');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Pinakas','Συμπλήρωση πίνακα','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('PinakesAl','Πίνακες αληθείας','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('PlRizSTim','Πλήθος ριζών - Σύνολο τιμών συνάρτησης','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('PlagAsym','Πλάγια ασύμπτωτη','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('PleurOr','Πλευρικά όρια συνάρτησης','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Synola');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('PollEpil','Πολλαπλής Επιλογής','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('PollKl','Πολλαπλασιασμός κλασμάτων - ακέραιου και κλάσματος','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Pollex','Πολλαπλασιασμός με e^x','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('Pollex','Πολλαπλασιασμός με e^x','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Polygwna','Πολύγωνα (βασικές έννοιες)','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('PrAkKl','Πρόσθεση ακέραιου με κλάσμα','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('PrEmbOgk','Προβλήματα εμβαδού - όγκου','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('PrFysikhs','Προβλήματα φυσικής','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('PrKlasm','Πρόσθεση - αφαίρεση κλασμάτων','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('PrOikonom','Προβλήματα οικονομίας','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('PrRhtPar','Πράξεις μεταξύ ρητών παραστάσεων','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('PraksDekAr','Πράξεις δεκαδικών αριθμών','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('PraksMon','Πράξεις μεταξύ μονωνύμων','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('PraksPol','Πράξεις μεταξύ πολυωνύμων','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('PraksSyn','Πράξεις μεταξύ συναρτήσεων','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('PraksSynol','Πράξεις συνόλων','Synola');
INSERT INTO "Sections_Exercises" VALUES ('ProbDian','Προβολή διανύσματος - Συνιστώσες','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Synola');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Problhmata','Προβλήματα','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('ProsShm','Προσδιορισμός σημείου','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('ProsSyn','Πρόσημο συνεχούς συνάρτησης','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('ProsTrigAr','Πρόσημο τριγωνομετρικού αριθμού σε κάθε τεταρτημόριο','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('ProsTriwn','Πρόσημο τριωνύμου','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('ProsdEuth','Προσδιορισμός ευθείας','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('ProsdPol','Προσδιορισμός πολυωνύμου','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('ProsdTim','Προσδιορισμός τιμής από συνέχεια','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('ProsthAr','Πρόσθεση αριθμών','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('ProsthDian','Πρόσθεση - Αφαίρεση διανυσμάτων','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('ProsthTm','Πρόσθεση ευθυγράμμων τμημάτων','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('PrwtoiAr','Πρώτοι αριθμοί - Πρώτοι μεταξύ τους','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Pyramida','Πυραμίδα','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('PythThewr','Πυθαγόρειο θεώρημα','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('RMetGwn','Ρυθμός μεταβολής και γωνία','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('RMetOmTr','Ρυθμός μεταβολής και όμοια τρίγωνα','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('Rabdogr','Ραβρόγραμμα συχνοτήτων','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('RhtopPar','Ρητοποίηση παρονομαστή','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('RhtopPar','Ρητοποίηση παρονομαστή','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('RizAnDiast','Ύπαρξη ρίζας σε ανοιχτό διάστημα','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('RizKlDiast','Ύπαρξη ρίζας σε κλειστό διάστημα','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('RizaExis','Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('RolleArx','Θ. Rolle με αρχική συνάρτηση','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Rombos','Ρόμβος','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('Sfaira','Σφαίρα','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('ShmKamp','Σημεία καμπής γραφικής παράστασης','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('ShmTom','Σημεία τομής ευθειών - Τομή με άξονες - Σημεία σε γραμμή','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('ShmTom','Σημεία τομής ευθειών - Τομή με άξονες - Σημεία σε γραμμή','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('ShmTom','Σημεία τομής ευθειών - Τομή με άξονες - Σημεία σε γραμμή','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('ShmXron','Σημειόγραμμα - Χρονόγραμμα συχνοτήτων','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('StSynGrPar','Στοιχεία από γραφική παράσταση','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('StSynGrPar','Στοιχεία από γραφική παράσταση','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('StahPrSyn','Συνάρτηση με σταθερό πρόσημο','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('StatSyn','Σταθερή συνάρτηση - Εύρεση τύπου','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('StathMesos','Σταθμικός μέσος','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('StathPros','Υπολογισμός απόλυτης τιμής με παράσταση σταθερού προσήμου','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('StoixEll','Στοιχεία έλλειψης (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('StoixParab','Στοιχεία παραβολής (Εξίσωση-εστία-διευθετούσα-παράμετρος p)','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('StoixPol','Στοιχεία κανονικού πολυγώνου (γωνία-κεντρική-πλευρά-περίμετρος-εμβαδόν)','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('StoixTrig','Στοιχεία τριγώνου','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('StoixYperb','Στοιχεία υπερβολής (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Synola');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('SwstoLathos','Σωστό - Λάθος','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('SxThEKyk','Σχετική θέση ευθείας και κύκλου','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('SxThEpip','Σχετικές θέσεις επιπέδων','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('SxThEuEp','Σχετική θέση ευθείας και επιπεέδου','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('SxThEuth','Σχετική θέση ευθειών - Ασύμβατες ευθείες','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('SxThGrPar','Σημεία τομής - Σχετική θέση γρ. παραστάσεων - Τομή με άξονες','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('SxThKykl','Σχετική θέση κύκλων','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('SxesOriz','Σχέσεις οριζουσών','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('SxetThEuth','Σχετική θέση ευθειών','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('SxhmTeth','Σχήμα - Τεθλασμένη γραμμή','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('SxhmaHor','Σχήμα Horner','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('SxhmaHor','Σχήμα Horner','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('SygkDekAr','Σύγκριση δεκαδικών αριθμών','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SygkFysAr','Σύγκριση φυσικών αριθμών','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SygkKlasm','Σύγκριση κλασμάτων','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SygkKlasm','Σύγκριση κλασμάτων','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SygkrAkKl','Σύγκριση ακεραίου με κλάσμα','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SygkrAr','Σύγκριση αριθμών','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('SygkrRiz','Σύγκριση άρρητων παραστάσεων','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('SygrAkAr','Σύγκριση ακέραιων αριθμών','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SymmPar','Συμμετρικές παραστάσεις ριζών x1-x2','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('SymmTet','Συμμετρίες - Τεταρτημόρια','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('SymplGwn','Συμπληρωματικές γωνίες','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Synola');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('SymplKenou','Συμπλήρωση κενού','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('SynGwnD','Συνημίτονο γωνίας διανυσμάτων','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('SynParall','Συνθήκη παραλληλίας διανυσμάτων','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('SynParall','Συνθήκη παραλληλίας διανυσμάτων','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('SynPollTyp','Συναρτήσεις πολλαπλού τύπου','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('SynPollTyp','Συναρτήσεις πολλαπλού τύπου','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('SynPollTyp','Συναρτήσεις πολλαπλού τύπου','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('SynSxesi','Συναρτησιακή σχέση','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('SynartSx','Συναρτησιακές σχέσεις','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('SyndMOl','Συνδυασμός μεθόδων ολοκλήρωσης','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Synepag','Συνεπαγωγή','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('SynexShm','Συνέχεια συνάρτησης σε σημείο','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('SynexSyn','Συνέχεια συνάρτησης σε σύνολο','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('SyntDian','Συντεταγμένες διανύσματος','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('SyntDieuth','Συντελεστής διεύθυνσης ευθείας - Γωνία','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('SyntDieuth','Συντελεστής διεύθυνσης ευθείας - Γωνία','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('SyntGnAk','Συντεταγμένες διαν. με γνωστά άκρα','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('SyntGrSynd','Συντεταγμένες γραμμικού συνδυασμού','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('SyntMes','Συντεταγμένες μέσου τμήματος - βαρύκεντρου','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('SyntelDieu','Συντελεστής διεύθυνσης διανύσματος','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Syntetag','Συντεταγμένες σημείων','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('SynthKl','Σύνθετα κλάσματα','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('SynthSyn','Σύνθεση συναρτήσεων','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('SynthSys','Σύνθετα συστήματα','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('SynthTrEx','Σύνθετες τριγωνομετρικές εξισώσεις','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Syst3x3','Συστήματα 3x3','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('Syzeuxh','Σύζευξη','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('TaytSynth','Ταυτότητες υπό συνθήκη','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('TeleiaDiair','Τέλεια διαίρεση πολυωνύμων','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('TemnKykl','Τέμνουσες κύκλου - Τέμνουσα και εφαπτομένη','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Tetragwno','Τετράγωνο','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('ThEsDix','Θεώρημα εσωτερική διχοτόμου - Τμήματα απέναντι πλευράς','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('ThExDix','Θεώρημα εξωτερικής διχοτόμου - Τμήματα απέναντι πλευράς','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('ThFermat','Θεώρημα Fermat','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('ThMT+','Συνδυασμός Θ.Μ.Τ. με άλλα θεωρήματα','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('ThThalh','Θεώρημα Θαλή','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('ThThalh','Θεώρημα Θαλή','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('ThYpsous','Θεώρημα ύψους προς την υποτείνουσα','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('TheorAkRiz','Σχήμα Horner - Θεώρημα ακέραιων ριζών','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('TheorAkRiz','Σχήμα Horner - Θεώρημα ακέραιων ριζών','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('TheorRiz','Θεώρημα ρίζας πολυωνύμου','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('TheorYpol','Θεώρημα υπολοίπου','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('ThewrYp','Θεωρήματα ύπαρξης','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Akolouth');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Anis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Anis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Anis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','AntistrSyn');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ApShm-Emb');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ApolTimh');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ArProodos');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Arithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ArritesAnEx');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ArxSAorOl');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Asymptotes');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','AthrGwnTr');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','AxKykloi');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','BasikesEnStat');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','DelHospital');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','DiaireshPolyon');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Dianysmata');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Diataxh');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','DionExiswseis');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EKP-MKD');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EKPAlgPar');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EfaptTmhmata');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EfekseisDiadox');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EggKanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EidhParall');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EkthetikhSyn');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Elleipsi');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EmbKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EmbXwriou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EmbadaBasSx');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EmbadaOgkoi');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EswterikoGin');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','EuthEpip3D');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Eutheia');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Exis1ou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Exis3ou+');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ExisEfaptom');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ExisKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','GenExisEutheias');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','GeomProodos');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','GinArithDian');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','GrammikaSys');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Gwnia');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','IsothtaKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','KanPolyg');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','KlasmExAnis');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Klasmata');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Kyrtothta');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','LogarSyn');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Logikh');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','LogosEmb');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MetatGrPar');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MetraDiasp');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MetraThesis');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Metyaxb');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MhGramSys');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MhPepOrio');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MhkosKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','MonotAkrot');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','NomosHmSyn');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','OmadParat');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Omoiothta');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Parabolh');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ParagShmeio');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Paragogos');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ParallEuth');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ParaplSymplG');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','PythTheor');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','RhtesParast');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','RythMetab');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','StatParDed');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SxetThKykl');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SxhmaTGr');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SygrKlasm');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SymmGraf');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Synart1p1');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SynepThMT');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SynexeiaSyn');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Synola');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','SyntetDian');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TemnKyklou');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ThBolzano');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ThDiameswn');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ThDixotomwn');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ThETΜΕΤ');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','ThThalh');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TrigSynart');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','TrigTaut');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Trigwna');
INSERT INTO "Sections_Exercises" VALUES ('Thewritikes','Θεωρητικές','Yperbolh');
INSERT INTO "Sections_Exercises" VALUES ('TimhMon','Τιμή μονωνύμου','Mononyma');
INSERT INTO "Sections_Exercises" VALUES ('TimhParast','Τιμή παράστασης - Σειρά πράξεων','PrakseisAr');
INSERT INTO "Sections_Exercises" VALUES ('TimhPol','Τιμή - Ρίζα πολυωνύμου','Polyonyma');
INSERT INTO "Sections_Exercises" VALUES ('TimhSyn','Τιμή συνάρτησης','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('TmMesaPl','Τμήμα που ενώνει τα μέσα πλευρών + αντίστροφο','Parallhlogr');
INSERT INTO "Sections_Exercises" VALUES ('TrAr2pi+','Τριγωνομετρικοί αριθμοί γωνιών μεγαλύτερων του κύκλου','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('TrAr2pi+','Τριγωνομετρικοί αριθμοί γωνιών μεγαλύτερων του κύκλου','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('TrArAth','Τριγωνομετρικοί αριθμοί αθροίσματος - διαφοράς','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('TrArDipl','Τριγωνομετρικοί αριθμοί διπλάσιας γωνίας','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('TrArTrig','Τριγωνομετρικοί αριθμοί γωνιών τριγώνου','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('TrArTripl','Τριγωνομετρικοί αριθμοί τριπλάσιας γωνίας','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('TrExArnAr','Τριγωνομετρική εξίσωση με αρνητικό αριθμό','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('TrExDiast','Τριγωνομετρική εξίσωση σε διάστημα','TrigExisoseis');
INSERT INTO "Sections_Exercises" VALUES ('Trapez','Τραπέζιο','Trapezio');
INSERT INTO "Sections_Exercises" VALUES ('TrigAnis','Τριγωνική ανισότητα - Κριτήριο ομόρροπων','ProsthDian');
INSERT INTO "Sections_Exercises" VALUES ('TrigAntik','Ολοκλήρωση με τριγωνομετρική αντικατάσταση','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('TrigBasGwn','Τριγωνομετρικοί αριθμοί βασικών γωνιών','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('TrigEx','Επίλυση τριγωνομετρικής εξίσωσης','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('TrigEx','Επίλυση τριγωνομετρικής εξίσωσης','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('TrigOlokl','Τριγωνομετρικά ολοκληρώματα','MethOlokl');
INSERT INTO "Sections_Exercises" VALUES ('TrigOrTr','Τριγωνομετρικοί αριθμοί σε ορθογώνιο τρίγωνο','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('TrigOria','Τριγωνομετρικά όρια','OrioApeiro');
INSERT INTO "Sections_Exercises" VALUES ('TrigOria','Τριγωνομετρικά όρια','OrioShmeio');
INSERT INTO "Sections_Exercises" VALUES ('TrigSysSyn','Τριγωνομετρικοί αριθμοί σε σύστημα συντεταγμένων','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('TrigonAnis','Τριγωνική ανισότητα','AnisTrig');
INSERT INTO "Sections_Exercises" VALUES ('TypoiVieta','Τύποι του Vieta','Exis2ou');
INSERT INTO "Sections_Exercises" VALUES ('XarGrafPar','Χάραξη γραφικής παράστασης','GrParastash');
INSERT INTO "Sections_Exercises" VALUES ('XarGrafPar','Χάραξη γραφικής παράστασης','Synart1ou');
INSERT INTO "Sections_Exercises" VALUES ('XarGrafPar','Χάραξη γραφικής παράστασης','Synart2ou');
INSERT INTO "Sections_Exercises" VALUES ('XarGrafPar','Χάραξη γραφικής παράστασης','SynartYper');
INSERT INTO "Sections_Exercises" VALUES ('XorToxAp','Χορδές - Τόξα - Αποστήματα κύκλου','Kyklos');
INSERT INTO "Sections_Exercises" VALUES ('XordApost','Χορδές και αποστήματα κύκλου','IsOrthTrig');
INSERT INTO "Sections_Exercises" VALUES ('XordToxa','Χορδές και τόξα κύκλου','IsotTrig');
INSERT INTO "Sections_Exercises" VALUES ('XwrDiast','Χωρισμός διαστήματος σε υποδιαστήματα','TheMTimhs');
INSERT INTO "Sections_Exercises" VALUES ('YpOrOlokl','Υπολογισμός ορισμένου ολοκληρώματος','OrismOlokl');
INSERT INTO "Sections_Exercises" VALUES ('YpOrOlokl','Υπολογισμός ορισμένου ολοκληρώματος','SynartOlokl');
INSERT INTO "Sections_Exercises" VALUES ('YpolLog','Υπολογισμός λογαρίθμων','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','Anag1oTet');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','DiaireshKlasm');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','Dynameis');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','Logarithmos');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','Paragont');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','PollKlasm');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','ProsthKlasm');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','Rizes');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','Tautothtes');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','TrigArAth');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','TrigArithmoi');
INSERT INTO "Sections_Exercises" VALUES ('YpolParast','Υπολογισμός παραστάσεων','TrigDiplG');
INSERT INTO "Sections_Exercises" VALUES ('Yposynolo','Υποσύνολο','Synola');
INSERT INTO "Sections_Exercises" VALUES ('YpshTr','Ύψη τριγώνου','GPythTheor');
INSERT INTO "Sections_Exercises" VALUES ('YpΝRiz','Ύπαρξη ν ριζών','ThRolle');
INSERT INTO "Sections_Exercises" VALUES ('Theoria','Θεωρία','Synarthsh');
INSERT INTO "Sections_Exercises" VALUES ('Probl','Προβλήματα','Pithanothta');
INSERT INTO "Sections_Exercises" VALUES ('PrEnd','Πράξεις ενδεχομένων','DeigmEnd');
INSERT INTO "Sections_Exercises" VALUES ('OrSys','Ορισμός συστήματος','GrammikaSys');
INSERT INTO "BackUp" VALUES ('Metadata','Id','Όνομα');
INSERT INTO "BackUp" VALUES ('Metadata','FileType','Τύπος αρχείου');
INSERT INTO "BackUp" VALUES ('Metadata','Field','Τομέας');
INSERT INTO "BackUp" VALUES ('Metadata','Chapter','Κεφάλαιο');
INSERT INTO "BackUp" VALUES ('Metadata','Section','Ενότητα');
INSERT INTO "BackUp" VALUES ('Metadata','ExerciseType','Είδος άσκησης');
INSERT INTO "BackUp" VALUES ('Metadata','Difficulty','Βαθμός δυσκολίας');
INSERT INTO "BackUp" VALUES ('Metadata','Path','Διαδρομή');
INSERT INTO "BackUp" VALUES ('Metadata','Date','Ημερομηνία');
INSERT INTO "BackUp" VALUES ('Metadata','Solved','Λυμένη');
INSERT INTO "BackUp" VALUES ('Metadata','Bibliography','Βιβλιογραφία');
INSERT INTO "BackUp" VALUES ('Metadata','FileContent','Περιεχόμενα');
INSERT INTO "BackUp" VALUES ('Metadata','Preamble','Preamble');
INSERT INTO "BackUp" VALUES ('Metadata','BuildCommand','Εντολή');
INSERT INTO "BackUp" VALUES ('Metadata','FileDescription','Περιγραφή');
INSERT INTO "BackUp" VALUES ('Metadata','MultiSection','Πολλαπλές ενότητες');
INSERT INTO "BackUp" VALUES ('Metadata','Class','Τάξη');
INSERT INTO "BackUp" VALUES ('Metadata','Year','Έτος');
INSERT INTO "BackUp" VALUES ('Metadata','Semester','Εξάμηνο');
INSERT INTO "BackUp" VALUES ('Bibliography','Citation_Key','Κλειδί αναφοράς');
INSERT INTO "BackUp" VALUES ('Bibliography','Document_Type','Τύπος εγγράφου');
INSERT INTO "BackUp" VALUES ('Bibliography','title','Τίτλος');
INSERT INTO "BackUp" VALUES ('Bibliography','author','Συγγραφέας');
INSERT INTO "BackUp" VALUES ('Bibliography','editor','Συντάκτης');
INSERT INTO "BackUp" VALUES ('Bibliography','publisher','Εκδότης');
INSERT INTO "BackUp" VALUES ('Bibliography','year','Έτος');
INSERT INTO "BackUp" VALUES ('Bibliography','month','Μήνας');
INSERT INTO "BackUp" VALUES ('Bibliography','isbn','ISBN');
INSERT INTO "BackUp" VALUES ('Bibliography','issn','ISSN');
INSERT INTO "BackUp" VALUES ('Bibliography','pages','Σελίδες');
INSERT INTO "BackUp" VALUES ('Bibliography','series','Σειρά');
INSERT INTO "BackUp" VALUES ('Bibliography','volume','Τόμος');
INSERT INTO "BackUp" VALUES ('Bibliography','journal','Περιοδικό');
INSERT INTO "BackUp" VALUES ('Bibliography','institution','Σχολείο/Ίδρυμα');
INSERT INTO "BackUp" VALUES ('Bibliography','issue','Αριθμός/Τεύχος');
INSERT INTO "BackUp" VALUES ('Bibliography','address','Διεύθυνση');
INSERT INTO "BackUp" VALUES ('Bibliography','doi','DOI');
INSERT INTO "BackUp" VALUES ('Bibliography','url','URL');
INSERT INTO "BackUp" VALUES ('Bibliography','edition','Έκδοση');
INSERT INTO "Chapters" VALUES ('Akol','Ακολουθίες','Algebra');
INSERT INTO "Chapters" VALUES ('AlgPar','Αλγεβρικές παραστάσεις','Algebra');
INSERT INTO "Chapters" VALUES ('Arith','Αριθμοί','Algebra');
INSERT INTO "Chapters" VALUES ('PaStD','Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων','Statistikh');
INSERT INTO "Chapters" VALUES ('Dian','Διανύσματα','Geometria');
INSERT INTO "Chapters" VALUES ('DEx1T','Διαφορικές εξισώσεις 1ης τάξης','Analysh');
INSERT INTO "Chapters" VALUES ('DEx2T','Διαφορικές εξισώσεις 2ης τάξης','Analysh');
INSERT INTO "Chapters" VALUES ('Logar','Εκθετική συνάρτηση - Λογάριθμός','Analysh');
INSERT INTO "Chapters" VALUES ('ExAn','Εξισώσεις - Ανισώσεις','Algebra');
INSERT INTO "Chapters" VALUES ('Eutheia','Ευθεία - Γωνίες','Geometria');
INSERT INTO "Chapters" VALUES ('Synart','Η έννοια της συνάρτησης - Βασικές συναρτήσεις','Analysh');
INSERT INTO "Chapters" VALUES ('Kykl','Κύκλος','Geometria');
INSERT INTO "Chapters" VALUES ('KwnT','Κωνικές τομές','Geometria');
INSERT INTO "Chapters" VALUES ('Logik','Λογική - Προτασιακός λογισμός','Algebra');
INSERT INTO "Chapters" VALUES ('MDE1T','Μερικές διαφορικές εξισώσεις 1ης τάξης','Analysh');
INSERT INTO "Chapters" VALUES ('MDE2T','Μερικές διαφορικές εξισώσεις 2ης τάξης','Analysh');
INSERT INTO "Chapters" VALUES ('Olokl','Ολοκληρώματα','Analysh');
INSERT INTO "Chapters" VALUES ('OlokEx','Ολοκληρωτικές εξισώσεις','Analysh');
INSERT INTO "Chapters" VALUES ('Omoia','Ομοιότητα - Αναλογίες','Geometria');
INSERT INTO "Chapters" VALUES ('Oria','Όρια','Analysh');
INSERT INTO "Chapters" VALUES ('Parag','Παράγωγοι','Analysh');
INSERT INTO "Chapters" VALUES ('Parall','Παραλληλόγραμμα','Geometria');
INSERT INTO "Chapters" VALUES ('Pithan','Πιθανότητες','Pithanothtes');
INSERT INTO "Chapters" VALUES ('Pinakes','Πίνακες','Algebra');
INSERT INTO "Chapters" VALUES ('KanP','Πολύγωνα - Κανονικά πολύγωνα','Geometria');
INSERT INTO "Chapters" VALUES ('PytTh','Πυθαγόρειο θεώρημα','Geometria');
INSERT INTO "Chapters" VALUES ('Stereom','Στερεομετρία','Geometria');
INSERT INTO "Chapters" VALUES ('Synex','Συνέχεια συνάρτησης','Analysh');
INSERT INTO "Chapters" VALUES ('Synola','Σύνολα','Pithanothtes');
INSERT INTO "Chapters" VALUES ('Syst','Συστήματα εξισώσεων και ανισώσεων','Algebra');
INSERT INTO "Chapters" VALUES ('SystSyn','Συστήματα συντεταγμένων','Geometria');
INSERT INTO "Chapters" VALUES ('Trig','Τρίγωνα','Geometria');
INSERT INTO "Chapters" VALUES ('Trigmt','Τριγωνομετρία','Algebra');
INSERT INTO "Chapters" VALUES ('BThTrig','Βασικά θεωρήματα στα τρίγωνα','Geometria');
INSERT INTO "Chapters" VALUES ('MetThDias','Μέτρα θέσης και διασποράς','Statistikh');
INSERT INTO "Chapters" VALUES ('-','-','-');
INSERT INTO "FileTypes" VALUES ('Def','Ορισμός','Ορισμοί',0,NULL);
INSERT INTO "FileTypes" VALUES ('Theor','Θεώρημα','Θεωρήματα',0,NULL);
INSERT INTO "FileTypes" VALUES ('Fig','Σχήμα','Σχήματα',0,NULL);
INSERT INTO "FileTypes" VALUES ('Tab','Πίνακας','Πίνακες',0,NULL);
INSERT INTO "FileTypes" VALUES ('SectEx','Άσκηση ενότητας','Ασκήσεις',1,NULL);
INSERT INTO "FileTypes" VALUES ('SolSE','Λύση άσκησης ενότητας','Λύσεις ασκήσεων ενότητας',-1,'SectEx');
INSERT INTO "FileTypes" VALUES ('SectSub','Θεμα ενότητας','Θεματα ενότητας',1,NULL);
INSERT INTO "FileTypes" VALUES ('SolSS','Λύση θέματος ενότητας','Λυσεις θεμάτων ενότητας',-1,'SectSub');
INSERT INTO "FileTypes" VALUES ('Method','Μέθοδος','Μέθοδοι',0,NULL);
INSERT INTO "FileTypes" VALUES ('Example','Λυμένο παράδειγμα','Λυμένα παραδείγματα',0,NULL);
INSERT INTO "FileTypes" VALUES ('CombEx','Συνδυαστική άσκηση','Συνδυαστικές ασκήσεις',1,NULL);
INSERT INTO "FileTypes" VALUES ('SolCE','Λύση συνδυαστικής άσκησης','Λυσεις συνδυαστικών ασκήσεων',-1,'CombEx');
INSERT INTO "FileTypes" VALUES ('CombSub','Συνδυαστικο θέμα','Συνδυαστικα θέματα',1,NULL);
INSERT INTO "FileTypes" VALUES ('SolCS','Λύση συνδυαστικού θέματος','Λυσεις συνδυαστικων θεμάτων',-1,'CombSub');
INSERT INTO "FileTypes" VALUES ('Prob','Πρόβλημα','Προβλήματα',1,NULL);
INSERT INTO "FileTypes" VALUES ('Prob-Sol','Πρόβλημα - Solution','Προβλήματα - Solutions',-1,'Prob');
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-ExisApolTim-SectEx1','@article','Algebra','','','','','','','','','','','','','','','','','',1);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-EpilExis-SectEx1','@book','1','2.0','3','4','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-EpilExis-SectEx2','@book','8','k','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-tes-ch11-sec22-ex11new-SectEx1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Exiswsh_2Ou_Bathmou','@article','8','2','3','','','','','','','','','','','','','','','',0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Geometrikh_Proodos',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Tautothta',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Tetragwnikh_Riza',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Dynamh_Pragm_Aritmou',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Yposynolo',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Akolouthia',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Dynamh_Me_Rhto_Ektheth',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Arithmitikh_Proodos',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-N_Osth_Riza',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Atithmitikos_Mesos',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Apolyth_Timh',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Synolo',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Or-Isa_Synola',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('S-Algebra-Gra-Theoria-SolSS1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-AorAdynEx-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-EpilExis-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-EpilExis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx9',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx8',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx10',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx7',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Logarithmos-YpolLog-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ArProodos-GenOrArPr-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ArProodos-GenOrArPr-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ArProodos-GenOrArPr-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ArProodos-GenOrArPr-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-KoinLys-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-KoinLys-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-KoinLys-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-SwstoLathos-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-ErwTheor-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-EpilAnis-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-EpilAnis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-EpilAnis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-EpilAnis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-SolSE2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigArithmoi-TrigBasGwn-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigArithmoi-TrigSysSyn-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigArithmoi-TrAr2pi+-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethAnt-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethAnt-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethAnt-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethAnt-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethAnt-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethOriz-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethOriz-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx7',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx8',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-Problhmata-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-GramExis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-GramExis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-GramExis-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-GramExis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethAntS-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-GrammikaSys-MethAntS-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-ExisAnt-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-ExisAnt-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-ExisAnt-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-ExisAnt-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-Parametrikes-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-Parametrikes-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-Parametrikes-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-Problhmata-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EpilExis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EpilExis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EpilExis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EpilExis-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-SwstoLathos-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-ErwTheor-SolSS1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-ErwTheor-SectSub1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-KoinLys-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SolSE2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis2ou-EpilAnis-SolSE3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-IdiotAnis-SectEx7',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-IdiotAnis-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-IdiotAnis-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-IdiotAnis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-IdiotAnis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-IdiotAnis-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-IdiotAnis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-SygkrAr-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-SygkrAr-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-SygkrAr-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-SygkrAr-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-ApodAnis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-ApodAnis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-MidAthr-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx9',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx7',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Diataxh-Diasthmata-SectEx8',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synarthsh',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Ises_Synarthseis_2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Grafikh_Parastash',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Topika_Akrotata',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Elaxisto',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Deuterh-N-osth_Paragwgos',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_shmeio',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Rythmos_Metabolhs',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Efaptomenh_Graf_Parastashs',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Prakseis_Synarthsewn',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_an_diasthma',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synolo_Timwn',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Gnisiws_Fthinousa',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synthesh_Synarthsewn',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_synolo',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Synarthsh_1-1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Ises_Synarthseis_1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Gnisiws_Auxousa',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Pragmatikh_Synarthsh',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Oliko_Megisto',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Olika_Akrotata',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Gnisiws_Monotonh',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Paragwgos_synarthsh',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Paragwgos_se_shmeio',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Topiko_Elaxisto',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Or-Topiko_Megisto',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-MonotAkrot-MonAkrGrPar-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-MonotAkrot-Monoton-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-MonotAkrot-AkrotAnis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-PedioOrism-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-PedioOrism-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-PedioOrism-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-PedioOrism-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-PedioOrism-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx8',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx14',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx11',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx16',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx15',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx12',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx9',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx13',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx7',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-LogarSyn-LogarExis-SectEx10',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-MonadRiz-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizKlDiast-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-BolzLim-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizaExis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizaExis-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizaExis-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-NRizes-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-MonadRiz-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizKlDiast-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-BolzLim-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizaExis-SolSE3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizaExis-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizaExis-SolSE2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-ThBolzano-NRizes-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-OrioShmeio-Or0p0Riz-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-OrioShmeio-Or0p0Riz-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParPollTyp-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParPollTyp-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagPilik-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagPilik-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagPilik-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParSynthRiz-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagSynth-SectEx7',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagSynth-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagSynth-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagSynth-SectEx6',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagSynth-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagSynth-SectEx4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagSynth-SectEx5',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagGin-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagGin-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParSynEkth-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParSynEkth-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParAnTax-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParAnTax-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParAnTax-SectEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Paragogos-ParagGin-SolSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Th-Grafikes_Par_Cf_Cf-1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-Method-Select file name',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-ExAn-Exis1ou-EpilExis-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-AkolouthAnis2ou-ThemaA-CombSub1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('test',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-HintSE1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Pithanothtes-Table-Endexomena',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Table-IdiothtesDynamewn',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Table-IdiothtesApolytwnTimwn',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Table-IdiothtesRizwn',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Figure-Diagramma_DionymEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Table-EidhRizwn',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-AlgPar-DiaireshPolyon-Antipar-SectEx',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Theorem-Proshmo_Triwnymou',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Analysh-Table-Eidh_Synarthsewn_1',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Definition-Select name',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Algebra-Table-Typoi_Ar_Geom_Proodou',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis1ou-AnisApT-SolSE4',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Anis3ou+-Method-(Select a name here)',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-test1-Definition-(Select a name here)',NULL,'8',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-test1-Definition-(test)','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition3','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition3','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem3','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-Method1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-Example1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition4','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition5','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition6','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition7','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Theorem1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition3','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition4','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition3','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition4','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition3','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Arith-Diataxh-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-GeomProodos-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-ArProodos-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-Akolouth-Definition1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-ArProodos-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-ArProodos-Definition3','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-ArProodos-Definition4','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-Akolouth-Definition2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx2','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Akol-Akolouth-Prob1','','8','','','','','','','','','','','','','','','','',NULL,0);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-ExisApolTim-SectEx1_1','@article','Equations2',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Synart-AntistrSyn-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def2','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def3','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Oria-Asymptotes-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Oria-Asymptotes-Def2','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Oria-Asymptotes-Def3','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Olokl-OrismOlokl-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def2','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def4','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('ΟρισμοίAnalysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('ΟρισμοίAnalysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Synart-Synarthsh-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def3','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def4','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-ParagShmeio-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-RythMetab-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Analysh-Parag-Paragogos-Def1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def4','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def5','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def6','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def7','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def8','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def9','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1','@article',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "Bibliography" VALUES ('Alg-Exis1ou-ExisApolTim-SectEx1_2','@article','Book title','Bob Writer','','Book Publications',2022,1,97852135484,'',524,'2','','','','','','','','',NULL);
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-AnisApT-SectEx1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-AnisApT-SectEx2','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE1-3','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE2','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE4','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx2','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx3','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx4','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-ErwTheor-SectEx1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx2','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx3','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis1ou-SwstoLathos-SectEx1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx1','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx2','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx1','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx2','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx3','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx4','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx5','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE1','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE2','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE3','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-KoinLys-SectEx1','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx1','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx2','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx3','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx4','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Anis3ou+-Method-(Select a name here)','Anis3ou+');
INSERT INTO "Sections_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx1','ApolTimh');
INSERT INTO "Sections_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx2','ApolTimh');
INSERT INTO "Sections_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx3','ApolTimh');
INSERT INTO "Sections_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx4','ApolTimh');
INSERT INTO "Sections_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx5','ApolTimh');
INSERT INTO "Sections_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx1','ArProodos');
INSERT INTO "Sections_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx2','ArProodos');
INSERT INTO "Sections_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx3','ArProodos');
INSERT INTO "Sections_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx4','ArProodos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-ApodAnis-SectEx1','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-ApodAnis-SectEx2','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx1','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx2','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx3','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx4','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx5','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx6','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx7','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx8','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx9','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx1','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx2','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx3','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx4','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx5','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx6','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx7','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-MidAthr-SectEx1','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx1','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx2','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx3','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx4','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('Alg-ExAn-Exis1ou-EpilExis-SectEx2','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-AorAdynEx-SectEx1','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-AorAdynEx-SolSE1-1','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx1','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx2','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx3','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx4','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-EpilExis-SolSE1-1','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-ExisApolTim-SectEx1','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-ExisApolTim-SolSE1-1','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis1ou-Method-Select file name','Exis1ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx2','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx3','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx4','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx5','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx6','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx2','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx3','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx4','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx2','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx3','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx4','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx2','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx3','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-Problhmata-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-SwstoLathos-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx2','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx3','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx4','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx5','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx6','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx3','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx4','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx3','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx4','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx5','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SolSE5-1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAntS-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethAntS-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethOriz-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-MethOriz-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx3','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE1-1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE2-1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE3-1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx3','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx4','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx5','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx6','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx7','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx8','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx1','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx10','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx2','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx3','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx4','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx5','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx6','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx7','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx8','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx9','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE1-1','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE10-1','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE4-1','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE5-1','Logarithmos');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx1','TrigArithmoi');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx2','TrigArithmoi');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx1','TrigArithmoi');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx2','TrigArithmoi');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigArithmoi-TrAr2pi+-SectEx1','TrigArithmoi');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigArithmoi-TrigBasGwn-SectEx1','TrigArithmoi');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigArithmoi-TrigSysSyn-SectEx1','TrigArithmoi');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx1','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx2','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx3','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx1','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx2','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx3','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx4','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx5','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx6','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx1','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx2','TrigExisoseis');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Figure-Diagramma_DionymEx1','DionExiswseis');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Akolouthia','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Apolyth_Timh','ApolTimh');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Arithmitikh_Proodos','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Atithmitikos_Mesos','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Dynamh_Me_Rhto_Ektheth','Dynameis');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Dynamh_Pragm_Aritmou','Dynameis');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Exiswsh_2Ou_Bathmou','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Geometrikh_Proodos','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Isa_Synola','Synola');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-N_Osth_Riza','Rizes');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Synolo','Synola');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Tautothta','Tautothtes');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Tetragwnikh_Riza','Rizes');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Or-Yposynolo','Synola');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Table-EidhRizwn','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Table-IdiothtesApolytwnTimwn','ApolTimh');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Table-IdiothtesDynamewn','Dynameis');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Table-IdiothtesRizwn','Rizes');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Table-Typoi_Ar_Geom_Proodou','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('Algebra-Theorem-Proshmo_Triwnymou','Anis2ou');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx1','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx10','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx11','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx12','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx13','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx14','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx15','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx16','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx2','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx3','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx4','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx5','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx6','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx7','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx8','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx9','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx1','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx2','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx3','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx4','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx5','LogarSyn');
INSERT INTO "Sections_per_File" VALUES ('Ana-MonotAkrot-AkrotAnis-SectEx1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Ana-MonotAkrot-MonAkrGrPar-SectEx1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Ana-MonotAkrot-Monoton-SectEx1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Ana-OrioShmeio-Or0p0Riz-SectEx1','OrioShmeio');
INSERT INTO "Sections_per_File" VALUES ('Ana-OrioShmeio-Or0p0Riz-SolSE1','OrioShmeio');
INSERT INTO "Sections_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx1','OrioShmeio');
INSERT INTO "Sections_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx2','OrioShmeio');
INSERT INTO "Sections_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx3','OrioShmeio');
INSERT INTO "Sections_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx4','OrioShmeio');
INSERT INTO "Sections_per_File" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx2','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx2','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx3','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParPollTyp-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParPollTyp-SectEx2','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParSynEkth-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParSynEkth-SectEx2','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParSynthRiz-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagGin-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagGin-SectEx2','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagGin-SolSE1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx2','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx3','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx2','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx3','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx4','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx5','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx6','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx7','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-BolzLim-SectEx1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-BolzLim-SolSE1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-MonadRiz-SectEx1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-MonadRiz-SolSE1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-NRizes-SectEx1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-NRizes-SolSE1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx2','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx3','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE2','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE3','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizKlDiast-SectEx1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizKlDiast-SolSE1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx2','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx3','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE2','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE3','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE1-1','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE1-1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE3-1','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE3-1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Deuterh-N-osth_Paragwgos','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Efaptomenh_Graf_Parastashs','ExisEfaptom');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Elaxisto','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Gnisiws_Auxousa','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Gnisiws_Fthinousa','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Gnisiws_Monotonh','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Grafikh_Parastash','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Ises_Synarthseis_1','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Olika_Akrotata','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Oliko_Megisto','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Paragwgos_se_shmeio','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Paragwgos_synarthsh','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Pragmatikh_Synarthsh','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Prakseis_Synarthsewn','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Rythmos_Metabolhs','RythMetab');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synarthsh','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synarthsh_1-1','AntistrSyn');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_an_diasthma','SynexeiaSyn');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma','SynexeiaSyn');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_shmeio','SynexeiaSyn');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_synolo','SynexeiaSyn');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synolo_Timwn','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Synthesh_Synarthsewn','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Topika_Akrotata','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Topiko_Elaxisto','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Or-Topiko_Megisto','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Table-Eidh_Synarthsewn_1','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('Analysh-Th-Grafikes_Par_Cf_Cf-1','AntistrSyn');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Definition1','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Definition2','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Prob1','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Akol-ArProodos-Definition1','ArProodos');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','Akolouth');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','Anis1ou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1','Paragont');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2','Paragont');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1','Paragont');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-Def1','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2','Polyonyma');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1','Tautothtes');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2','Tautothtes');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3','Tautothtes');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4','Tautothtes');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def1','Arithmoi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def2','Arithmoi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def3','Arithmoi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Tab1','Arithmoi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Arith-Diataxh-Definition1','Diataxh');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SolSE2-1','Exis2ou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Example1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Method1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1','GrammikaSys');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','AntistrSyn');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Synart1p1');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','SynexeiaSyn');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','GrParastash');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','SynthSyn');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','ThETΜΕΤ');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','IsesSyn');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','OrioShmeio');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','SynthSyn');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Olokl-OrismOlokl-Def1','OrismOlokl');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def1','Asymptotes');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def2','Asymptotes');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def3','Asymptotes');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def1','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def2','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def3','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def4','Kyrtothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def2','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def3','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def4','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-ParagShmeio-Def1','ParagShmeio');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-Def1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-Tab1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Parag-RythMetab-Def1','RythMetab');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Synart-AntistrSyn-Def1','AntistrSyn');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-Synart-Synarthsh-Def1','Synarthsh');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','MonotAkrot');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Paragogos');
INSERT INTO "Sections_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','ThBolzano');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE1-1','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE2-1','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE3-1','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2','EswterikoGin');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SolSE1-1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE1-1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE2-1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SolSE1-1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2','SyntetDian');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','EmbadaBasSx');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','PythTheor');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1','ExisEutheias');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SolSE1-1','ExisEutheias');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1','Elleipsi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1','Elleipsi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2','Elleipsi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE1-1','Elleipsi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE2-1','Elleipsi');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE1-1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE2-1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE3-1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE4-1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE5-1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SolSE1-1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1','ExisKyklou');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition1','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition2','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition3','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition4','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition5','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition6','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition7','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SolSE1-1','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition1','Pithanothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition2','Pithanothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1','Pithanothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem1','Pithanothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem2','Pithanothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem3','Pithanothta');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition1','MetraDiasp');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition2','MetraDiasp');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition3','MetraDiasp');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition4','MetraDiasp');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1','MetraDiasp');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition1','MetraThesis');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition2','MetraThesis');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition3','MetraThesis');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def4','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def5','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def6','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def7','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def8','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def9','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition1','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition2','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition3','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Theorem1','BasEnnStat');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition1','ParStD');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition2','ParStD');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition3','ParStD');
INSERT INTO "Sections_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition4','ParStD');
INSERT INTO "Sections_per_File" VALUES ('Pithanothtes-Table-Endexomena','DeigmEnd');
INSERT INTO "Sections_per_File" VALUES ('1','Exis1ou');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-AnisApT-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-AnisApT-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE1-3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-ErwTheor-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis1ou-SwstoLathos-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx5','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-KoinLys-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Anis3ou+-Method-(Select a name here)','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx2','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx3','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx4','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx5','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx1','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx2','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx3','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx4','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-ApodAnis-SectEx1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-ApodAnis-SectEx2','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx2','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx3','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx4','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx5','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx6','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx7','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx8','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx9','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx2','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx3','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx4','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx5','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx6','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx7','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-MidAthr-SectEx1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx2','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx3','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx4','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Alg-ExAn-Exis1ou-EpilExis-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-AorAdynEx-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-AorAdynEx-SolSE1-1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-EpilExis-SolSE1-1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-ExisApolTim-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-ExisApolTim-SolSE1-1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis1ou-Method-Select file name','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx5','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx6','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-Problhmata-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-SwstoLathos-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx3','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx4','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx5','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx6','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx3','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx4','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx3','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx4','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx5','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SolSE5-1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAntS-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethAntS-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethOriz-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-MethOriz-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx3','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE1-1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE2-1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE3-1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx3','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx4','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx5','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx6','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx7','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx8','Syst');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx1','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx10','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx2','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx3','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx4','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx5','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx6','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx7','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx8','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx9','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE1-1','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE10-1','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE4-1','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE5-1','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx2','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx2','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigArithmoi-TrAr2pi+-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigArithmoi-TrigBasGwn-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigArithmoi-TrigSysSyn-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx2','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx3','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx2','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx3','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx4','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx5','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx6','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx1','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx2','Trigmt');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Figure-Diagramma_DionymEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Akolouthia','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Apolyth_Timh','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Arithmitikh_Proodos','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Atithmitikos_Mesos','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Dynamh_Me_Rhto_Ektheth','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Dynamh_Pragm_Aritmou','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Exiswsh_2Ou_Bathmou','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Geometrikh_Proodos','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Isa_Synola','Synola');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-N_Osth_Riza','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Synolo','Synola');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Tautothta','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Tetragwnikh_Riza','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Or-Yposynolo','Synola');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Table-EidhRizwn','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Table-IdiothtesApolytwnTimwn','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Table-IdiothtesDynamewn','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Table-IdiothtesRizwn','Arith');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Table-Typoi_Ar_Geom_Proodou','Akol');
INSERT INTO "Chapters_per_File" VALUES ('Algebra-Theorem-Proshmo_Triwnymou','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx1','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx10','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx11','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx12','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx13','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx14','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx15','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx16','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx2','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx3','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx4','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx5','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx6','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx7','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx8','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx9','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx1','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx2','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx3','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx4','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx5','Logar');
INSERT INTO "Chapters_per_File" VALUES ('Ana-MonotAkrot-AkrotAnis-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-MonotAkrot-MonAkrGrPar-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-MonotAkrot-Monoton-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-OrioShmeio-Or0p0Riz-SectEx1','Oria');
INSERT INTO "Chapters_per_File" VALUES ('Ana-OrioShmeio-Or0p0Riz-SolSE1','Oria');
INSERT INTO "Chapters_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx1','Oria');
INSERT INTO "Chapters_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx2','Oria');
INSERT INTO "Chapters_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx3','Oria');
INSERT INTO "Chapters_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx4','Oria');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx3','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParPollTyp-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParPollTyp-SectEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParSynEkth-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParSynEkth-SectEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParSynthRiz-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagGin-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagGin-SectEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagGin-SolSE1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx3','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx3','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx4','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx5','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx6','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx7','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-BolzLim-SectEx1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-BolzLim-SolSE1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-MonadRiz-SectEx1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-MonadRiz-SolSE1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-NRizes-SectEx1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-NRizes-SolSE1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx2','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx3','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE2','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE3','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizKlDiast-SectEx1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizKlDiast-SolSE1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx2','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx3','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE2','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE3','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE1-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE1-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE3-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE3-1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Deuterh-N-osth_Paragwgos','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Efaptomenh_Graf_Parastashs','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Elaxisto','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Gnisiws_Auxousa','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Gnisiws_Fthinousa','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Gnisiws_Monotonh','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Grafikh_Parastash','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Ises_Synarthseis_1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Olika_Akrotata','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Oliko_Megisto','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Paragwgos_se_shmeio','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Paragwgos_synarthsh','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Pragmatikh_Synarthsh','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Prakseis_Synarthsewn','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Rythmos_Metabolhs','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synarthsh','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synarthsh_1-1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_an_diasthma','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_shmeio','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_synolo','Synex');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synolo_Timwn','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Synthesh_Synarthsewn','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Topika_Akrotata','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Topiko_Elaxisto','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Or-Topiko_Megisto','Parag');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Table-Eidh_Synarthsewn_1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('Analysh-Th-Grafikes_Par_Cf_Cf-1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1','Akol');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Definition1','Akol');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Definition2','Akol');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Prob1','Akol');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Akol-ArProodos-Definition1','Akol');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','Akol');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-Def1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4','AlgPar');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def2','Arith');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def3','Arith');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Tab1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Arith-Diataxh-Definition1','Arith');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SolSE2-1','ExAn');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Example1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Method1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1','Syst');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Oria');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Olokl-OrismOlokl-Def1','Olokl');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def1','Oria');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def2','Oria');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def3','Oria');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def3','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def4','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def2','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def3','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def4','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-ParagShmeio-Def1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-Def1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-Tab1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Parag-RythMetab-Def1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Synart-AntistrSyn-Def1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-Synart-Synarthsh-Def1','Synart');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Parag');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Synex');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE1-1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE2-1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE3-1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SolSE1-1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE1-1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE2-1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SolSE1-1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2','Dian');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','KanP');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','Trig');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1','Eutheia');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SolSE1-1','Eutheia');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE1-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE2-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE1-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE2-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE3-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE4-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE5-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SolSE1-1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1','KwnT');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition1','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition2','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition3','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition4','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition5','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition6','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition7','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SolSE1-1','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition1','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition2','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem1','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem2','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem3','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition1','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition2','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition3','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition4','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition1','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition2','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition3','MetThDias');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def4','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def5','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def6','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def7','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def8','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def9','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition1','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition2','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition3','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Theorem1','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition1','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition2','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition3','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition4','PaStD');
INSERT INTO "Chapters_per_File" VALUES ('Pithanothtes-Table-Endexomena','Pithan');
INSERT INTO "Chapters_per_File" VALUES ('1','ExAn');
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-ExisApolTim-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Ασκήσεις/Εξισώσεις με απόλυτες τιμές/Alg-Exis1ou-ExisApolTim-SectEx1.tex','14/10/2020 14:56','YES','@article{Alg-Exis1ou-ExisApolTim-SectEx1_2,
	title = {Book title},
	author = {Bob Writer},
	publisher = {Book Publications},
	year = {2022},
	month = {1},
	isbn = {97852135484},
	pages = {524},
	series = {2},
}

@book{Alg-Exis1ou-ExisApolTim-SectEx1_2,
	title = {Book title},
	author = {Bob Writer},
	publisher = {Book Publications},
	year = {2022},
	month = {1},
	isbn = {97852135484},
	pages = {524},
	series = {2},
}

','%# Database File : Alg-Exis1ou-ExisApolTim-SectEx1
%@ Database source: Mathematics
\item Να λυθούν οι εξισώσεις.
\begin{multicols}{4}
\begin{alist}
\item $ \left|x-3\right|=x+2 $
\item $ \left|4x-1\right|=2x-5 $
\item $ \left|2x-3\right|=4-7x $
\item $ \left|\dfrac{x}{2}-1\right|=\dfrac{x+3}{4} $
\end{alist}
\end{multicols}
%# End of file Alg-Exis1ou-ExisApolTim-SectEx1
','Basic','PdfLaTeX','Μια καινούρια άσκηση',0,'Α'' Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Exiswsh_2Ou_Bathmou','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Exiswsh_2Ou_Bathmou.tex','09/7/2020 17:55','','@article{Algebra-Or-Exiswsh_2Ou_Bathmou,
	title = {1},
	author = {2},
	editor = {3},
}','%# Database File : Algebra-Or-Exiswsh_2Ou_Bathmou
%@ Database source: Mathematics
Εξίσωση 2ου βαθμού ονομάζεται κάθε εξίσωση της μορφής:
\[ ax^2+\beta x+\gamma=0\ ,\ a\neq 0 \]
%# End of file Algebra-Or-Exiswsh_2Ou_Bathmou','Basic','PdfLaTeX',NULL,0,'','','');
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Geometrikh_Proodos','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Geometrikh_Proodos.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Geometrikh_Proodos
%@ Database source: Mathematics
Γεωμετρική πρόοδος ονομάζεται κάθε ακολουθία $ (a_\nu),\nu\in\mathbb{N}^* $ πραγματικών αριθμών στην οποία κάθε όρος της προκύπτει πολλαπλασιάζοντας κάθε φορά τον προηγούμενο όρο με τον ίδιο σταθερό αριθμό. Θα ισχύει
\[ a_{\nu+1}=\lambda\cdot a_\nu \]
Ο αριθμός $ \lambda $ ονομάζεται \textbf{λόγος} της γεωμετρικής προόδου και είναι σταθερός. Ισούται με \[ \lambda=\frac{a_{\nu+1}}{a_\nu} \]
%# End of file Algebra-Or-Geometrikh_Proodos.dos.','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Tautothta','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Tautothta.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Tautothta
%@ Database source: Mathematics
Ταυτότητα ονομάζεται κάθε ισότητα που περιέχει μεταβλητές και επαληθεύεται για κάθε τιμή των μεταβλητών.
%# End of file Algebra-Or-Tautothta','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Tetragwnikh_Riza','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Tetragwnikh_Riza.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Tetragwnikh_Riza
%@ Database source: Mathematics
Τετραγωνική ρίζα ενός μη αρνητικού αριθμού $x$ ονομάζεται ο μη αρνητικός αριθμός $a$ ο οποίος αν υψωθεί στο τετράγωνο δίνει τον αριθμό $ x $. Συμβολίζεται με $ \sqrt{x} $ και είναι
\[ \sqrt{x}=a\ \ ,\ \ x\geq 0,\ a\geq 0 \]
\begin{itemize}[itemsep=0mm]
\item Ο αριθμός $ x $ ονομάζεται \textbf{υπόριζο}.
\item Δεν ορίζεται ρίζα αρνητικού αριθμού.
\end{itemize}
%# End of file Algebra-Or-Tetragwnikh_Riza','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Dynamh_Pragm_Aritmou','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Dynamh_Pragm_Aritmou.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Dynamh_Pragm_Aritmou
%@ Database source: Mathematics
Δύναμη ενός πραγματικού αριθμού $ a $ ονομάζεται το γινόμενο $ \nu $ ίσων παραγόντων του αριθμού αυτού. Συμβολίζεται με $ a^\nu $ όπου $ \nu\in\mathbb{N} $ είναι το πλήθος των ίσων παραγόντων. 
\[ \undercbrace{a\cdot a\cdot\ldots a}_{\nu\textrm{ παράγοντες }}=a^\nu \]
Ο αριθμός $ a $ ονομάζεται \textbf{βάση} και ο αριθμός $ \nu $ \textbf{εκθέτης} της δύναμης.
%# End of file Algebra-Or-Dynamh_Pragm_Aritmou','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Yposynolo','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Yposynolo.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Yposynolo
%@ Database source: Mathematics
Ένα σύνολο $ A $ λέγεται υποσύνολο ενός συνόλου $ B $ όταν κάθε στοιχείο του $ A $ είναι και στοιχείο του $ B $. Συμβολίζεται με τη χρήση του συμβόλου $ \subseteq $ ως εξής : $ A\subseteq B $.
%# End of file Algebra-Or-Yposynolo','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Akolouthia','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Akolouthia.tex','06/10/2020 12:23','','','%# Database File : Algebra-Or-Akolouthia
%@ Database source: Mathematics
Ακολουθία πραγματικών αριθμών ονομάζεται κάθε συνάρτηση της μορφής $ a:\mathbb{N}^*\rightarrow\mathbb{R} $ όπου κάθε φυσικός αριθμός $ \nu\in\mathbb{N}^* $, εκτός του μηδενός, αντιστοιχεί σε ένα πραγματικό αριθμό $ a(\nu)\in\mathbb{R} $ ή πιο απλά $ a_\nu $.
\begin{itemize}[itemsep=0mm]
\item Η ακολουθία των πραγματικών αριθμών συμβολίζεται $ \left( a_\nu\right)  $.
\item Οι πραγματικοί αριθμοί $ a_1, a_2,\ldots,a_\nu $ ονομάζονται \textbf{όροι} της ακολουθίας.
\item Ο όρος $ a_\nu $ ονομάζεται \textbf{ν-οστός} ή \textbf{γενικός} όρος της ακολουθίας.
\item Οι όροι μιας ακολουθίας μπορούν να δίνονται είτε από 
\begin{itemize}[itemsep=0mm]
\item έναν \textbf{γενικό τύπο} της μορφής $ a_\nu=f(\nu) $, όπου δίνεται κατευθείαν ο γενικός όρος της
\item είτε από \textbf{αναδρομικό τύπο} όπου κάθε όρος δίνεται με τη βοήθεια ενός ή περισσότερων προηγούμενων όρων. Θα είναι της μορφής \[ a_{\nu+i}=f(a_{\nu+i-1},\ldots,a_{\nu+1},a_\nu)\;\;,\;\;a_1,a_2,\ldots,a_i\textrm{ γνωστοί όροι.} \]
Στον αναδρομικό τύπο, ο αριθμός $ i\in\mathbb{N} $ είναι το πλήθος των προηγούμενων όρων από τους οποίους εξαρτάται ο όρος $ a_{\nu+i} $. Είναι επίσης αναγκαίο να γνωρίζουμε τις τιμές των $ i $ πρώτων όρων της προκειμένου να υπολογίσουμε τους υπόλοιπους.
\end{itemize}
\item Μια ακολουθία της οποίας όλοι οι όροι είναι ίσοι ονομάζεται \textbf{σταθερή}.
\end{itemize}
%# End of file Algebra-Or-Akolouthia','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Dynamh_Me_Rhto_Ektheth','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Dynamh_Me_Rhto_Ektheth.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Dynamh_Me_Rhto_Ektheth
%@ Database source: Mathematics
Δύναμη ενός \textbf{θετικού} αριθμού $ a $ με εκθέτη ένα ρητό αριθμό $ \frac{\mu}{\nu} $, όπου $ \mu\in\mathbb{Z} $ και $ \nu\in\mathbb{N}^* $, ορίζεται ως η ρίζα $ \nu $-τάξης του αριθμού $ a $ υψωμένο στη δύναμη $ \mu $.
\[ a^{\frac{\mu}{\nu}}=\!\sqrt[\nu]{a^\mu}\ ,\ \textrm{ όπου } a>0 \]
%# End of file Algebra-Or-Dynamh_Me_Rhto_Ektheth','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Arithmitikh_Proodos','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Arithmitikh_Proodos.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Arithmitikh_Proodos
%@ Database source: Mathematics
Αριθμητική πρόοδος ονομάζεται κάθε ακολουθία $ (a_\nu),\nu\in\mathbb{N}^* $ πραγματικών αριθμών στην οποία κάθε όρος της προκύπτει από τον προηγούμενο, προσθέτοντας κάθε φορά τον ίδιο σταθερό αριθμό. Ισχύει δηλαδή
\[ a_{\nu+1}=a_\nu+\omega \]
Ο αριθμός $ \omega $ ονομάζεται \textbf{διαφορά} της αριθμητικής προόδου και είναι σταθερός. Ισούται με 
\[ \omega=a_{\nu+1}-a_\nu  \]
%# End of file Algebra-Or-Arithmitikh_Proodos','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-N_Osth_Riza','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-N_Osth_Riza.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-N_Osth_Riza
%@ Database source: Mathematics
Ρίζα $ \nu $-οστής \textbf{τάξης} ενός μη αρνητικού αριθμού $ x $ ονομάζεται ο \textbf{μη αρνητικός} αριθμός $ a $ που αν υψωθεί στη δύναμη $ \nu $ δίνει αποτέλεσμα $ x $ (υπόριζο). Συμβολίζεται με $ \sqrt[\nu]{x} $.
\[ \sqrt[\nu]{x}=a\;\;,\;\;\textrm{ όπου }x\geq0\textrm{ και }a\geq0 \]
%# End of file Algebra-Or-N_Osth_Riza','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Atithmitikos_Mesos','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Atithmitikos_Mesos.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Atithmitikos_Mesos
%@ Database source: Mathematics
Αριθμητικός μέσος τριών διαδοχικών όρων $ a,\beta,\gamma $ μιας αριθμητικής προόδου $ (a_\nu) $ ονομάζεται ο μεσαίος όρος $ \beta $ για τον οποίο έχουμε \[ 2\beta=a+\gamma\;\;\textrm{ ή }\;\;\beta=\frac{a+\gamma}{2} \]
Γενικότερα, αριθμητικός μέσος $ \nu $ διαδοχικών όρων $ a_1,a_2,\ldots,a_\nu $ μιας αριθμητικής προόδου ονομάζεται ο πραγματικός αριθμός \[ \mu=\frac{a_1+a_2+\ldots+a_\nu}{\nu} \]
%# End of file Algebra-Or-Atithmitikos_Mesos','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Apolyth_Timh','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Apolyth_Timh.tex','27/11/2020 01:37','','','%# Database File : Algebra-Or-Apolyth_Timh
%@ Database source: Mathematics
Απόλυτη τιμή ενός πραγματικού αριθμού $ a\in\mathbb{R} $ ονομάζεται η απόσταση του σημείου $ A $ του αριθμού από την αρχή $ O $ του άξονα των πραγματικών αριθμών. Συμβολίζεται με $ |a| $.
\begin{center}
\begin{tabular}{c >{\centering\arraybackslash}m{6cm}}
$ |a|=\begin{cases}
\begin{aligned}
a & \;,\;a\geq0\\
-a & \;,\;a<0
\end{aligned}
\end{cases} $  & \begin{tikzpicture}
\draw[-latex] (-1,0) -- coordinate (x axis mid) (4.4,0) node[right,fill=white] {{\footnotesize $ x $}};
\draw (0,.5mm) -- (0,-.5mm) node[anchor=north,fill=white] {{\scriptsize $ 0 $}};
\draw (3,.5mm) -- (3,-.5mm) node[anchor=north,fill=white] {{\scriptsize $ a $}};
\draw[line width=.7mm] (0,0) -- (3,0);
\tkzText(1.5,.34){$ \overcbrace{\rule{28mm}{0mm}}^{{\scriptsize |a|}} $}
\tkzDefPoint(3,0){A}
\tkzDrawPoint(A)
\tkzLabelPoint[above right](A){{\scriptsize $A(a)$}}
\tkzDrawPoint(0,0)
\tkzLabelPoint[above left](0,0){{\scriptsize $O(0)$}}
\end{tikzpicture}
\end{tabular} 
\end{center}
Η απόλυτη τιμή ενός θετικού αριθμού $ a $ είναι ίση με τον ίδιο τον αριθμό ενώ η απόλυτη τιμή ενός αρνητικού αριθμού $ a $ είναι ίση με τον αντίθετο του δηλαδή $ -a $.
%# End of file Algebra-Or-Apolyth_Timh','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Synolo','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Synolo.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Synolo
%@ Database source: Mathematics
Σύνολο ονομάζεται μια συλλογή όμοιων αντικειμένων, τα οποία είναι καλά ορισμένα και διακριτά μεταξύ τους.
\begin{itemize}[itemsep=0mm]
\item Τα αντικείμενα ενός συνόλου ονομάζονται \textbf{στοιχεία}.
\item Τα σύνολα τα συμβολίζουμε με ένα κεφαλαίο γράμμα.
\item Για να δηλώσουμε ότι ένα στοιχείο $ x $ \textbf{ανήκει} σε ένα σύνολο $ A $ γράφουμε $ x\in A $. Ενώ αν το $ x $ \textbf{δεν ανήκει} στο σύνολο $ A $ γράφουμε $ x\notin A $.
\end{itemize}
%# End of file Algebra-Or-Synolo','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Or-Isa_Synola','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ορισμοί/Algebra-Or-Isa_Synola.tex','09/7/2020 17:55','','','%# Database File : Algebra-Or-Isa_Synola
%@ Database source: Mathematics
Ίσα ονομάζονται δύο σύνολα $ A,B $ τα οποία έχουν ακριβώς τα ίδια στοιχεία. Συμβολίζεται $ A=B $. Ισοδύναμα, τα σύνολα $ Α,Β $ λέγονται ίσα εάν ισχύουν οι σχέσεις :
\begin{enumerate}[itemsep=0mm]
\item Κάθε στοιχείο του $ A $ είναι και στοιχείο του $ B $
\item Κάθε στοιχείο του $ B $ είναι και στοιχείο του $ A $.
\end{enumerate}
%# End of file Algebra-Or-Isa_Synola','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-AorAdynEx-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Ασκήσεις/Αόριστες και αδύνατες εξισώσεις 1ου βαθμού/Alg-Exis1ou-AorAdynEx-SectEx1.tex','09/11/2020 20:51','YES','','%# Database File : Alg-Exis1ou-AorAdynEx-SectEx1
%@ Database source: Mathematics
Να λύσετε τις παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ 3x+4=2(x-1)+x-5 $
\item $ 4x-(3-2x)=2x+9 $
\item $ 8-(4-x)=2x-(x-3) $
\item $ 2(x+3)-14=21-5(3-x)-3x $
\end{alist}
\end{multicols}
%# End of file Alg-Exis1ou-AorAdynEx-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-EpilExis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis1ou-EpilExis-SectEx1.tex','09/11/2020 20:21','YES','','%# Database File : Alg-Exis1ou-EpilExis-SectEx1
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ 2x-4=0 $
\item $ 12-4x=4 $
\item $ 3x+4=x-8 $
\item $ 5x-7=2x+5 $
\end{alist}
\end{multicols}
%# End of file Alg-Exis1ou-EpilExis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-EpilExis-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis1ou-EpilExis-SectEx4.tex','09/11/2020 20:50','ΟΧΙ','','%# Database File : Alg-Exis1ou-EpilExis-SectEx4
%@ Database source: Mathematics
Να λύσετε τις παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ 2(x+4)-4x=5x-(7-2x) $
\item $ 3(1-3x)+5=12-7x $
\end{alist}
\end{multicols}
%# End of file Alg-Exis1ou-EpilExis-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-EpilExis-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis1ou-EpilExis-SectEx3.tex','09/11/2020 20:34','ΟΧΙ','','%# Database File : Alg-Exis1ou-EpilExis-SectEx3
%@ Database source: Mathematics
Να λύσετε τις παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ \dfrac{x-2}{3}+\dfrac{x}{2}=1 $
\item $ \dfrac{1-3x}{4}+\dfrac{x-5}{3}=1-\dfrac{x}{12} $
\item $ 1-\dfrac{2-x}{10}=\dfrac{x-3}{5} $
\item $ \dfrac{4x+7}{4}+1=\dfrac{3-x}{8}+\dfrac{2x+1}{4} $
\end{alist}
\end{multicols}
%# End of file Alg-Exis1ou-EpilExis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-EpilExis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis1ou-EpilExis-SectEx2.tex','09/11/2020 20:27','ΟΧΙ','','%# Database File : Alg-Exis1ou-EpilExis-SectEx2
%@ Database source: Mathematics
Να λύσετε τις παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ 3(x-1)+4=7-(4-x) $
\item $ 2(1-x)=8+3(2x-4) $
\item $ -(7-x)-2(3x+1)=15 $
\item $ 9+2(x-2)=4-(3x-7) $
\end{alist}
\end{multicols}
%# End of file Alg-Exis1ou-EpilExis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx1.tex','16/7/2020 17:53','YES','','%# Database File : Alg-Logarithmos-YpolLog-SectEx1
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{4}
\begin{rlist}[leftmargin=2mm]
\item $ \log_{2}{4} $
\item $ \log_{3}{9} $
\item $ \log_{5}{125} $
\item $ \log_{2}{16} $
\item $ \log_{3}{27} $
\item $ \log_{4}{16} $
\item $ \log_{2}{32} $
\item $ \log_{2}{64} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx2.tex','16/7/2020 18:01','ΟΧΙ','','%# Database File : Alg-Logarithmos-YpolLog-SectEx2
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}[leftmargin=5mm]
\item $ \log{100} $
\item $ \log{10000} $
\item $ \log{10^7} $
\item $ \log{10^{-19}} $
\item $ \ln{e^2} $
\item $ \ln{e^{-23}} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx4.tex','16/7/2020 18:03','YES','','%# Database File : Alg-Logarithmos-YpolLog-SectEx4
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}
\item $ \log{\frac{1}{10}} $
\item $ \log{\frac{1}{1000}} $
\item $ \log{\frac{1}{10^{-3}}} $
\item $ \ln{\frac{1}{e}} $
\item $ \ln{\frac{1}{e^5}} $
\item $ \ln{\frac{1}{e^{-4}}} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx5.tex','16/7/2020 18:04','YES','','%# Database File : Alg-Logarithmos-YpolLog-SectEx5
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}[leftmargin=2mm]
\item $ \log_{2}{0{,}25} $
\item $ \log_{2}{0{,}125} $
\item $ \log_{5}{0{,}04} $
\item $ \log_{8}{0{,}125} $
\item $ \log{0{,}0001} $
\item $ \log_{100}{0{,}01} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx3.tex','16/7/2020 18:02','ΟΧΙ','','%# Database File : Alg-Logarithmos-YpolLog-SectEx3
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}
\item $ \log_{2}{\frac{1}{4}} $
\item $ \log_{2}{\frac{1}{32}} $
\item $ \log_{3}{\frac{1}{9}} $
\item $ \log_{3}{\frac{1}{81}} $
\item $ \log_{4}{\frac{1}{64}} $
\item $ \log_{8}{\frac{1}{512}} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx9','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx9.tex','16/7/2020 18:08','ΟΧΙ','','%# Database File : Alg-Logarithmos-YpolLog-SectEx9
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}[leftmargin=3mm]
\item $ \log_{\sqrt{2}}{4} $
\item $ \log_{\sqrt{3}}{3} $
\item $ \log_{\sqrt{5}}{25} $
\item $ \log_{\sqrt{e}}{e^3} $
\item $ \log_{\sqrt{2}}{4\sqrt{2}} $
\item $ \log_{\sqrt[3]{4}}{2} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx9','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx8','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx8.tex','16/7/2020 18:07','ΟΧΙ','','%# Database File : Alg-Logarithmos-YpolLog-SectEx8
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}[leftmargin=2mm]
\item $ \log_{\frac{4}{3}}{\frac{9}{16}} $
\item $ \log_{\frac{8}{5}}{\frac{125}{512}} $
\item $ \log_{\frac{1}{10}}{1000} $
\item $ \log_{\frac{1}{2}}{16} $
\item $ \log_{\frac{1}{5}}{625} $
\item $ \log_{\frac{1}{4}}{256} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx8','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx10','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx10.tex','16/7/2020 23:16','YES','','%# Database File : Alg-Logarithmos-YpolLog-SectEx10
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}[leftmargin=2mm]
\item $ \log_{2}{(\sqrt[3]{2})} $
\item $ \log_{2}{(\sqrt[4]{8})} $
\item $ \log_{3}{(\sqrt[3]{9})} $
\item $ \log_{4}{\frac{1}{2\sqrt[5]{16}}} $
\item $ \log_{5}{(\sqrt[3]{5})} $
\item $ \log_{5}{\frac{1}{\sqrt{125}}} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx10','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx7','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx7.tex','16/7/2020 18:06','ΟΧΙ','','%# Database File : Alg-Logarithmos-YpolLog-SectEx7
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}[leftmargin=2mm]
\item $ \log_{\frac{3}{2}}{\frac{9}{4}} $
\item $ \log_{\frac{1}{4}}{\frac{1}{64}} $
\item $ \log_{\frac{5}{7}}{\frac{125}{343}} $
\item $ \log_{\frac{1}{10}}{\frac{1}{10000}} $
\item $ \log_{\frac{4}{e}}{\frac{16}{e^2}} $
\item $ \log_{\frac{e}{10}}{\frac{e^3}{1000}} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx7','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SectEx6','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Ασκήσεις/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SectEx6.tex','16/7/2020 18:05','ΟΧΙ','','%# Database File : Alg-Logarithmos-YpolLog-SectEx6
%@ Database source: Mathematics
Να υπολογίσετε την τιμή των παρακάτω λογαρίθμων.
\begin{multicols}{3}
\begin{rlist}[leftmargin=2mm]
\item $ \log_{0{,}1}{0{,}01} $
\item $ \log_{0{,}2}{0{,}008} $
\item $ \log_{0{,}3}{0{,}0081} $
\item $ \log_{1{,}5}{2{,}25} $
\item $ \log_{0{,}4}{6{,}25} $
\item $ \log_{0{,}5}{8} $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ArProodos-GenOrArPr-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Αριθμητική πρόοδος/Ασκήσεις/Γενικός όρος αριθμητικής προόδου - Εύρεση όρων και βήματος ω/Alg-ArProodos-GenOrArPr-SectEx1.tex','14/10/2020 15:37','ΟΧΙ','','%# Database File : Alg-ArProodos-GenOrArPr-SectEx1
%@ Database source: Mathematics
Να βρεθεί ο ν\textsuperscript{ος} όρος των παρακάτω αριθμητικών προόδων.
\begin{multicols}{3}
\begin{alist}
\item $ 1,3,5,\ldots $
\item $ 0,2,4,\ldots $
\item $ 3,8,13,\ldots $
\item $ -5,-2,1,\ldots $
\item $ \dfrac{1}{2},2,\frac{7}{2}\ldots $
\end{alist}
\end{multicols}
%# End of file Alg-ArProodos-GenOrArPr-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ArProodos-GenOrArPr-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Αριθμητική πρόοδος/Ασκήσεις/Γενικός όρος αριθμητικής προόδου - Εύρεση όρων και βήματος ω/Alg-ArProodos-GenOrArPr-SectEx3.tex','14/10/2020 15:37','ΟΧΙ','','%# Database File : Alg-ArProodos-GenOrArPr-SectEx3
%@ Database source: Mathematics
Να βρεθεί ο πρώτος όρος και η διαφορά $ \mathbold\omega $ κάθε αριθμητικής προόδου όταν :
\begin{multicols}{2}
\begin{alist}
\item $ a_3=9 $ και $ a_{8}=24 $
\item $ a_7=18 $ και $ a_{18}=40 $
\item $ a_{12}=30 $ και $ a_{25}=72 $
\item $ a_{14}=21 $ και $ a_{30}=-11 $
\end{alist}
\end{multicols}
%# End of file Alg-ArProodos-GenOrArPr-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ArProodos-GenOrArPr-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Αριθμητική πρόοδος/Ασκήσεις/Γενικός όρος αριθμητικής προόδου - Εύρεση όρων και βήματος ω/Alg-ArProodos-GenOrArPr-SectEx2.tex','14/10/2020 15:37','ΟΧΙ','','%# Database File : Alg-ArProodos-GenOrArPr-SectEx2
%@ Database source: Mathematics
Να βρεθεί ο ζητούμενος όρος κάθε αριθμητικής προόδου.
\begin{multicols}{2}
\begin{alist}
\item Ο $ a_9 $ της $ 0,3,6,\ldots $
\item Ο $ a_{15} $ της $ -2,3,8,\ldots $
\item Ο $ a_{21} $ της $ 17,29,41,\ldots $
\item Ο $ a_{20} $ της $ 42,35,28,\ldots $
\item Ο $ a_{50} $ της $ -50,-35,-20\ldots $
\item Ο $ a_{38} $ της $ -40,-10,20,\ldots $
\end{alist}
\end{multicols}
%# End of file Alg-ArProodos-GenOrArPr-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ArProodos-GenOrArPr-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Αριθμητική πρόοδος/Ασκήσεις/Γενικός όρος αριθμητικής προόδου - Εύρεση όρων και βήματος ω/Alg-ArProodos-GenOrArPr-SectEx4.tex','14/10/2020 15:30','ΟΧΙ','','%# Database File : Alg-ArProodos-GenOrArPr-SectEx4
%@ Database source: Mathematics
Να βρεθεί ποιος όρος από την παρακάτω ακολουθία είναι ίσος με το δοσμένο $ \nu- $οστό όρο.
\begin{multicols}{2}
\begin{alist}
\item $ 4,7,10\ldots $ με $ a_{\nu}=157 $
\item $ -2,5,12\ldots $ με $ a_\nu=236 $
\item $ 21,35,49\ldots $ με $ a_\nu=567 $
\item $ 128,104,80\ldots $ με $ a_\nu=-1600 $
\item $ -2,-6,-10\ldots $ με $ a_\nu=-122 $
\item $ \dfrac{3}{2},\dfrac{7}{4},2\ldots $ με $ a_\nu=8 $
\end{alist}
\end{multicols}
%# End of file Alg-ArProodos-GenOrArPr-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-AnisApT-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Ανισώσεις με απόλυτες τιμές/Alg-Anis1ou-AnisApT-SectEx1.tex','07/11/2020 16:26','YES','','%# Database File : Alg-Anis1ou-AnisApT-SectEx1
%@ Database source: Mathematics
Να λυθούν οι ανισώσεις....
\begin{multicols}{4}
\begin{alist}
\item $ \left|x\right|<4 $
\item $ \left|x\right|>5 $
\item $ \left|x-1\right|<2 $
\item $ \left|x+2\right|>3 $
\item $ \left|2x-1\right|\leq5 $
\item $ \left|3x+4\right|\geq8 $
\item $ \left|1-x\right|<2 $
\item $ \left|3-4x\right|\geq5 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-AnisApT-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-AnisApT-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Ανισώσεις με απόλυτες τιμές/Alg-Anis1ou-AnisApT-SectEx2.tex','14/10/2020 14:53','ΝΑΙ','','%# Database File : Alg-Anis1ou-AnisApT-SectEx2
%@ Database source: Mathematics
\item Να λυθούν οι ανισώσεις.
\begin{multicols}{4}
\begin{alist}
\item $ \left|x\right|<-2 $
\item $ \left|4x\right|>-1 $
\item $ \left|x-3\right|\leq0 $
\item $ \left|2x-4\right|\geq0 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-AnisApT-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-KoinLys-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Κοινές λύσεις ανισώσεων και διπλών ανισώσεων/Alg-Anis1ou-KoinLys-SectEx3.tex','14/10/2020 15:00','ΟΧΙ','','%# Database File : Alg-Anis1ou-KoinLys-SectEx3
%@ Database source: Mathematics
Να λυθούν οι ανισώσεις και να παρασταθούν γραφικά οι λύσεις και να γραφτούν με τη μορφή διαστήματος.
\begin{multicols}{2}
\begin{alist}
\item $ 4x-3<3x<2-5x $
\item $ 3-2x\leq x+1<4x-5 $
\item $ 3(1-x+2)<4x\leq2(x+2)+3 $
\item $ 5(2x-1)-4\leq 7(3-x)\leq 4(2-x)+3x $
\item $ 3-(3x-4)<2(x-2)+4(3-x)<7-(x-3) $
\item $ \dfrac{x-1}{2}<\dfrac{x}{3}+1\leq\dfrac{2x-1}{2}-\dfrac{1}{3} $
\item $ \dfrac{3x-4}{5}\leq\dfrac{2-x}{3}<\dfrac{x}{15}+1 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-KoinLys-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-KoinLys-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Κοινές λύσεις ανισώσεων και διπλών ανισώσεων/Alg-Anis1ou-KoinLys-SectEx1.tex','14/10/2020 14:51','ΟΧΙ','','%# Database File : Alg-Anis1ou-KoinLys-SectEx1
%@ Database source: Mathematics
Να βρεθούν οι κοινές λύσεις των ανισώσεων και να γραφτούν με τη μορφή διαστήματος.
\begin{multicols}{2}
\begin{alist}
\item $ 3x-1>5 $ και $ 4x-3<9 $
\item $ 4-3x<2 $ και $ 2x+5\leq7 $
\item $ 2(x-3)+5>x-1 $ και $ 3-(x-4)\leq5-2x $
\item $ 4(x-2)+3(5-x)\geq4x-3+2(x-1) $ και $ 5(2-x)+3(x+1)<4-(x-7) $
\item $ \dfrac{x+5}{12}+1\geq\dfrac{x}{4} $ και $ \dfrac{2x+3}{4}+\dfrac{x-1}{3}>1 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-KoinLys-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-KoinLys-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Κοινές λύσεις ανισώσεων και διπλών ανισώσεων/Alg-Anis1ou-KoinLys-SectEx2.tex','14/10/2020 14:54','ΟΧΙ','','%# Database File : Alg-Anis1ou-KoinLys-SectEx2
%@ Database source: Mathematics
Να βρεθούν οι κοινές λύσεις των ανισώσεων και να γραφτούν με τη μορφή διαστήματος.
\begin{multicols}{3}
\begin{alist}
\item $ 2\leq\left|x\right|\leq3 $
\item $ 3\leq\left|x-1\right|\leq7 $
\item $4\leq\left|2x-4\right|\leq8 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-KoinLys-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-SwstoLathos-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Σωστό - Λάθος/Alg-Anis1ou-SwstoLathos-SectEx1.tex','14/10/2020 14:44','ΟΧΙ','','%# Database File : Alg-Anis1ou-SwstoLathos-SectEx1
%@ Database source: Mathematics
\begin{alist}
\item Η ανίσωση $ ax+\beta>0 $ με $ a>0 $ έχει μια λύση την $ x>-\dfrac{\beta}{a} $.
\item Η ανίσωση $ ax+\beta>0 $ με $ a=0 $ και $ \beta>0 $ είναι αόριστη.
\begin{multicols}{2}
\item Η αν. $ 0x<\beta $ με $ \beta>0 $ είναι αδύνατη.
\item Η αν. $ 0x<\beta $ με $ \beta<0 $ είναι αδύνατη.
\item Η αν. $ 0x>\beta $ με $ \beta=0 $ είναι αόριστη.
\item Η αν. $ 0x\geq\beta $ με $ \beta=0 $ είναι αόριστη.
\item Η αν. $ 0x>\beta $ με $ \beta>0 $ είναι αδύνατη.
\item Η αν. $ 0x\leq\beta $ με $ \beta=0 $ είναι αδύνατη.
\item Η αν. $ 0x<-\beta $ με $ \beta>0 $ είναι αόριστη.
\item Η αν. $ 0x\geq-\beta $ με $ \beta=0 $ είναι αόριστη.
\end{multicols}
\end{alist}
%# End of file Alg-Anis1ou-SwstoLathos-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-ErwTheor-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Ερωτήσεις θεωρίας/Alg-Anis1ou-ErwTheor-SectEx1.tex','14/10/2020 14:43','ΟΧΙ','','%# Database File : Alg-Anis1ou-ErwTheor-SectEx1
%@ Database source: Mathematics
\begin{alist}
\item Τι ονομάζουμε ανίσωση 1\textsuperscript{ου} βαθμού;
\item Πότε μια ανίσωση ονομάζεται αδύνατη;
\item Πότε μια ανίσωση ονομάζεται αόριστη;
\end{alist}
%# End of file Alg-Anis1ou-ErwTheor-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-EpilAnis-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis1ou-EpilAnis-SectEx4.tex','09/11/2020 15:52','ΟΧΙ','','%# Database File : Alg-Anis1ou-EpilAnis-SectEx4
%@ Database source: Mathematics
Να λυθούν οι ανισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ 3x-2<x+4+2x $
\item $ 4x-3+x\geq2x-3+2x $
\item $ 2(x-3)+1<-3x+5(x-2) $
\item $ 4x-(3+2x)>5(x-2)+3(2-x)+1 $
\item $ 5-(x-2)+3x\leq3(2+x)-x-1 $
\item $ \dfrac{2x-3}{4}-\dfrac{x}{2}>1 $
\item $ \dfrac{3x-4}{5}-\dfrac{x-3}{3}\geq\dfrac{x-1}{15}+\dfrac{x-4}{5} $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-EpilAnis-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-EpilAnis-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis1ou-EpilAnis-SectEx3.tex','09/11/2020 15:51','ΟΧΙ','','%# Database File : Alg-Anis1ou-EpilAnis-SectEx3
%@ Database source: Mathematics
Να λυθούν οι ανισώσεις και να παρασταθούν γραφικά οι λύσεις.
\begin{multicols}{2}
\begin{alist}
\item $ \dfrac{x}{2}+\dfrac{x+1}{3}>1 $
\item $ \dfrac{2x-1}{3}-\dfrac{x-2}{4}<\dfrac{1}{6} $
\item $ \dfrac{x}{5}+\dfrac{3x-2}{3}\leq\dfrac{x-1}{15} $
\item $ \dfrac{4x-3}{3}-\dfrac{3-2x}{4}\geq1+\dfrac{5x}{12} $
\vfill
\columnbreak
\vfill
\item $ 2x-\dfrac{3x-2}{5}+\dfrac{x-1}{15}\leq\dfrac{1}{3}-\dfrac{2-3x}{15} $
\item $ \dfrac{-2-x}{4}+\dfrac{4x-5}{8}<3x-1-\dfrac{7-4x}{4} $
\item $ \dfrac{1-\dfrac{x}{2}}{3}>2 $
\item $ \dfrac{\dfrac{x-1}{3}+\dfrac{x-2}{4}}{2}-\dfrac{2x-1}{6}>\dfrac{x}{12} $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-EpilAnis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-EpilAnis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis1ou-EpilAnis-SectEx1.tex','07/11/2020 16:26','ΟΧΙ','','%# Database File : Alg-Anis1ou-EpilAnis-SectEx1
%@ Database source: Mathematics
Να λυθούν οι ανισώσεις και να παρασταθούν γραφικά οι λύσεις.
\begin{multicols}{3}
\begin{alist}
\item $ 2x-3>7-3x $
\item $ 4x+5<2-x+8 $
\item $ 3x-2\leq4-2x+8 $
\item $ -x-4\geq7-3x+2 $
\item $ 7x-3+x<2x+9+5x $
\item $ -3x+8>4-5x+12 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-EpilAnis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-EpilAnis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis1ou-EpilAnis-SectEx2.tex','09/11/2020 15:51','ΟΧΙ','','%# Database File : Alg-Anis1ou-EpilAnis-SectEx2
%@ Database source: Mathematics
Να λυθούν οι ανισώσεις και να παρασταθούν γραφικά οι λύσεις.
\begin{multicols}{2}
\begin{alist}
\item $ 2(x-1)+3>4-x $
\item $ 2x-3(4-x)<9+4x $
\item $ 4(3-x)+2(3x-1)<3x+2-(x-1) $
\item $ 3(2x+3)-5>5(x-4)+12 $
\item $ -2-3(4-3x)+5x\leq3-(7-2x) $
\item $ 2-(3x-4)+x\geq3(2x+3)-12-(x-2) $
\end{alist}
\end{multicols}
%# End of file Alg-Anis1ou-EpilAnis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-AnisApT-SolSE2','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Λύσεις ασκήσεων ενότητας/Ανισώσεις με απόλυτες τιμές/Alg-Anis1ou-AnisApT-SolSE2.tex',NULL,'ΟΧΙ','','%# Database File : Alg-Anis1ou-AnisApT-SolSE2
%@ Database source: Mathematics
\begin{alist}
\item Η ανίσωση $ |x|<-2 $ είναι αδύνατη.
\item Η ανίσωση $ |4x|\geq-1 $ επαληθεύεται για κάθε $ x\in\mathbb{R} $.
\item $ |x-3|\leq 0\Rightarrow |x-3|=0\Rightarrow x-3=0\Rightarrow x=3 $.
\item Η ανίσωση $ |2x-4|\geq 0 $ επαληθεύεται για κάθε $ x\in\mathbb{R} $.
\end{alist}
%# End of file Alg-Anis1ou-AnisApT-SolSE2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-AnisApT-SolSE1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Λύσεις ασκήσεων ενότητας/Ανισώσεις με απόλυτες τιμές/Alg-Anis1ou-AnisApT-SolSE1.tex','15/12/2020 01:09','ΟΧΙ','','%# Database File : Alg-Anis1ou-AnisApT-SolSE1
%@ Database source: Mathematics
h
%# End of file Alg-Anis1ou-AnisApT-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Τριγωνομετρική εξίσωση με αρνητικό αριθμό/Alg-TrigExisoseis-TrExArnAr-SectEx1.tex','15/10/2020 12:00','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-TrExArnAr-SectEx1
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ \hm{x}=-\frac{1}{2} $
\item $ \hm{x}=-\frac{\sqrt{2}}{2} $
\item $ \syn{x}=-\frac{1}{2} $
\item $ \syn{x}=-\frac{\sqrt{3}}{2} $
\end{alist}
\end{multicols}
%# End of file Alg-TrigExisoseis-TrExArnAr-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Τριγωνομετρική εξίσωση με αρνητικό αριθμό/Alg-TrigExisoseis-TrExArnAr-SectEx2.tex','15/10/2020 12:01','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-TrExArnAr-SectEx2
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ \ef{x}=-\frac{\sqrt{3}}{3} $
\item $ \syf{x}=-1 $
\item $ \syf{x}=-\sqrt{3} $
\item $ \ef{x}=-\sqrt{3} $
\end{alist}
\end{multicols}
%# End of file Alg-TrigExisoseis-TrExArnAr-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Απλές τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-AplTrExis-SectEx2.tex','17/10/2020 16:19','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-AplTrExis-SectEx2
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ \syn{x}=\frac{\sqrt{2}}{2} $
\item $ \syn{x}=\frac{\sqrt{3}}{2} $
\item $ \syn{x}=0 $
\item $ \syn{x}=1 $
\end{alist}
\end{multicols}
%# End of file Alg-TrigExisoseis-AplTrExis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Απλές τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-AplTrExis-SectEx1.tex','17/10/2020 16:49','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-AplTrExis-SectEx1
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ \hm{x}=\frac{1}{2} $
\item $ \hm{x}=\frac{\sqrt{2}}{2} $
\item $ \hm{x}=1 $
\item $ \hm{x}=\frac{\sqrt{3}}{2} $
\end{alist}
\end{multicols}


%# End of file Alg-TrigExisoseis-AplTrExis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Απλές τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-AplTrExis-SectEx3.tex','17/10/2020 16:19','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-AplTrExis-SectEx3
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ \ef{x}=\frac{\sqrt{3}}{3} $
\item $ \syf{x}=\sqrt{3} $
\item $ \ef{x}=1 $
\item $ \syf{x}=\frac{\sqrt{3}}{3} $
\end{alist}
\end{multicols}
%# End of file Alg-TrigExisoseis-AplTrExis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx6','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Σύνθετες τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-SynthTrEx-SectEx6.tex','15/10/2020 12:05','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-SynthTrEx-SectEx6
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=2mm]
\item $ \hm{\left( \frac{\pi}{2}-x\right) }=\frac{\sqrt{2}}{2} $
\item $ \syn{\left(\frac{\pi}{2}-x\right) }=-\frac{1}{2} $
\item $ \ef{\left(\frac{\pi}{2}-x\right) }=\frac{\sqrt{3}}{3} $
\item $ \syf{\left(\frac{\pi}{2}-x \right) }=1 $
\end{alist}
\end{multicols}
%# End of file Alg-TrigExisoseis-SynthTrEx-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Σύνθετες τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-SynthTrEx-SectEx1.tex','15/10/2020 11:56','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-SynthTrEx-SectEx1
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{alist}[leftmargin=3mm]
\begin{multicols}{2}
\item $ \hm{(2x)}=\frac{\sqrt{2}}{2} $
\item $ \hm{(3x)}=\frac{\sqrt{3}}{2} $
\item $ \hm{(x+\pi)}=\frac{1}{2} $
\item $ \hm{\left(2x+\frac{\pi}{3}\right)}=1 $
\end{multicols}
\end{alist}
%# End of file Alg-TrigExisoseis-SynthTrEx-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Σύνθετες τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-SynthTrEx-SectEx2.tex','15/10/2020 11:57','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-SynthTrEx-SectEx2
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{alist}[leftmargin=3mm]
\begin{multicols}{2}
\item $ \syn{(3x)}=\frac{\sqrt{3}}{2} $
\item $ \syn{(2x)}=\frac{1}{2} $
\item $ \syn{\left( x+\frac{\pi}{4}\right) }=1 $
\item $ \syn{\left(4x+\frac{\pi}{6}\right)}=\frac{\sqrt{3}}{2} $
\end{multicols}
\end{alist}
%# End of file Alg-TrigExisoseis-SynthTrEx-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Σύνθετες τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-SynthTrEx-SectEx5.tex','15/10/2020 12:04','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-SynthTrEx-SectEx5
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=2mm]
\item $ \ef{\left( \pi-x\right) }=\frac{\sqrt{3}}{3} $
\item $ \syf{\left(\pi-x\right) }=0 $
\item $ \syf{\left(\pi+x\right) }=-\frac{\sqrt{3}}{3} $
\item $ \ef{\left(\pi+x \right) }=1 $
\end{alist}
\end{multicols}
%# End of file Alg-TrigExisoseis-SynthTrEx-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Σύνθετες τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-SynthTrEx-SectEx3.tex','15/10/2020 11:58','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-SynthTrEx-SectEx3----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{alist}[leftmargin=3mm]
\begin{multicols}{2}
\item $ \ef{(2x)}=\frac{\sqrt{3}}{3} $
\item $ \syf{(5x)}=1 $
\item $ \syf{\left( 3x+\frac{3\pi}{4}\right) }=\sqrt{3} $
\item $ \ef{\left(3x+\frac{\pi}{2}\right)}=\frac{\sqrt{3}}{3} $
\end{multicols}
\end{alist}
%# End of file Alg-TrigExisoseis-SynthTrEx-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικές εξισώσεις/Ασκήσεις/Σύνθετες τριγωνομετρικές εξισώσεις/Alg-TrigExisoseis-SynthTrEx-SectEx4.tex','15/10/2020 12:03','ΟΧΙ','','%# Database File : Alg-TrigExisoseis-SynthTrEx-SectEx4----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=2mm]
\item $ \hm{\left( \pi-x\right) }=\frac{1}{2} $
\item $ \syn{\left(\pi-x\right) }=-1 $
\item $ \syn{\left(\pi+x\right) }=-\frac{\sqrt{2}}{2} $
\item $ \hm{\left(\pi+x \right) }=\frac{\sqrt{3}}{2} $
\end{alist}
\end{multicols}
%# End of file Alg-TrigExisoseis-SynthTrEx-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Απόλυτη τιμή/Ασκήσεις/Μήκος - κέντρο και ακτίνα διαστήματος/Alg-ApolTimh-MhkKenAkt-SectEx1.tex','08/10/2020 20:24','ΟΧΙ','','%# Database File : Alg-ApolTimh-MhkKenAkt-SectEx1----
%@ Database source: Mathematics
Να βρεθούν το μήκος, το κέντρο και η ακτίνα των παρακάτω διαστημάτων.
\begin{multicols}{2}
\begin{alist}
\item $ [1,5] $
\item $ (-2,4) $
\item $ [-10,-1) $
\item $ (0,8] $
\item $ \left(\frac{1}{2},\frac{5}{4}\right) $
\item $ \left[\frac{3}{8},2\right] $
\end{alist}
\end{multicols}
%# End of file Alg-ApolTimh-MhkKenAkt-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Απόλυτη τιμή/Ασκήσεις/Μήκος - κέντρο και ακτίνα διαστήματος/Alg-ApolTimh-MhkKenAkt-SectEx4.tex','08/10/2020 20:36','ΟΧΙ','','%# Database File : Alg-ApolTimh-MhkKenAkt-SectEx4----
%@ Database source: Mathematics
Η ακτίνα του διαστήματος $ [\lambda-1,3] $ είναι $ 4 $.
\begin{alist}
\item Να βρεθεί η τιμή του $ \lambda $.
\item Να βρεθεί το μήκος και το κέντρο του διαστήματος.
\end{alist}
%# End of file Alg-ApolTimh-MhkKenAkt-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Απόλυτη τιμή/Ασκήσεις/Μήκος - κέντρο και ακτίνα διαστήματος/Alg-ApolTimh-MhkKenAkt-SectEx2.tex','08/10/2020 20:27','ΟΧΙ','','%# Database File : Alg-ApolTimh-MhkKenAkt-SectEx2----
%@ Database source: Mathematics
Το κέντρο του διαστήματος $ [1,\lambda] $ είναι το $ 4 $. Να βρεθεί ο πραγματικός αριθμός $ \lambda $ καθώς και το μήκος και η ακτίνα του διαστήματος.
%# End of file Alg-ApolTimh-MhkKenAkt-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Απόλυτη τιμή/Ασκήσεις/Μήκος - κέντρο και ακτίνα διαστήματος/Alg-ApolTimh-MhkKenAkt-SectEx5.tex','09/10/2020 00:21','ΟΧΙ','','%# Database File : Alg-ApolTimh-MhkKenAkt-SectEx5----
%@ Database source: Mathematics
Το διάστημα $ [2\lambda+3,2-\lambda] $, όπου $ \lambda\in\mathbb{R} $, έχει αντίθετα άκρα. Να βρεθούν 
\begin{alist}
\item η τιμή της παραμέτρου $ \lambda $.
\item το κέντρο, το μήκος και η ακτίνα του διαστήματος.
\end{alist}
%# End of file Alg-ApolTimh-MhkKenAkt-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Απόλυτη τιμή/Ασκήσεις/Μήκος - κέντρο και ακτίνα διαστήματος/Alg-ApolTimh-MhkKenAkt-SectEx3.tex','08/10/2020 20:29','ΟΧΙ','','%# Database File : Alg-ApolTimh-MhkKenAkt-SectEx3----
%@ Database source: Mathematics
Το μήκος του διαστήματος $ [\lambda-1,\lambda^2] $ είναι $ 3 $, όπου $ \lambda\in\mathbb{R} $.
\begin{alist}
\item Να βρεθεί η τιμή του $ \lambda $.
\item Να βρεθεί το κέντρο και η ακτίνα του διαστήματος.
\end{alist}
%# End of file Alg-ApolTimh-MhkKenAkt-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigArithmoi-TrigBasGwn-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικοί αριθμοί/Ασκήσεις/Τριγωνομετρικοί αριθμοί βασικών γωνιών/Alg-TrigArithmoi-TrigBasGwn-SectEx1.tex','15/7/2020 21:27','ΟΧΙ','','%# Database File : Alg-TrigArithmoi-TrigBasGwn-SectEx1----
%@ Database source: Mathematics
Να υπολογίσετε τις παρακάτω αριθμητικές παραστάσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=4mm]
\item $ \hm{30\degree}\cdot\hm{60\degree} $
\item $ \hm^2{40\degree}-2\syn{60\degree} $
\item $ \ef{45\degree}+2\syn^2{30\degree} $
\item $ \syf^2{60\degree}-\hm^2{60\degree} $
\end{alist}
\end{multicols}
%# End of file Alg-TrigArithmoi-TrigBasGwn-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigArithmoi-TrigSysSyn-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικοί αριθμοί/Ασκήσεις/Τριγωνομετρικοί αριθμοί σε σύστημα συντεταγμένων/Alg-TrigArithmoi-TrigSysSyn-SectEx1.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-TrigArithmoi-TrigSysSyn-SectEx1----
%@ Database source: Mathematics
Να υπολογίσετε τους τριγωνομετρικούς αριθμούς της γωνίας $ x\hat{O}M $ η οποία σχηματίζεται μέσα σε ένα ορθοκανονικό σύστημα συντεταγμένων $ xOy $ για καθένα από τα παρακάτω σημεία $ M $.
\begin{multicols}{2}
\begin{rlist}[leftmargin=4mm]
\item $ M(3,4) $
\item $ M(5,12) $
\item $ M(-8,15) $
\item $ M(6,-8) $
\item $ M(-4,-3) $
\item $ M(12,-9) $
\end{rlist}
\end{multicols}
%# End of file Alg-TrigArithmoi-TrigSysSyn-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigArithmoi-TrAr2pi+-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικοί αριθμοί/Ασκήσεις/Τριγωνομετρικοί αριθμοί γωνιών μεγαλύτερων του κύκλου/Alg-TrigArithmoi-TrAr2pi+-SectEx1.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-TrigArithmoi-TrAr2pi+-SectEx1----
%@ Database source: Mathematics
Να υπολογίσετε τους τριγωνομετρικούς αριθμούς των παρακάτω γωνιών.
\begin{multicols}{3}
\begin{rlist}
\item $ 390\degree $
\item $ 450\degree $
\item $ 780\degree $
\item $ 1260\degree $
\item $ 1125\degree $
\item $ 1845\degree $
\end{rlist}
\end{multicols}
%# End of file Alg-TrigArithmoi-TrAr2pi+-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικοί αριθμοί/Ασκήσεις/Μετατροπή μοιρών σε ακτίνια και αντίστροφα/Alg-TrigArithmoi-MoirAkt-SectEx2.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-TrigArithmoi-MoirAkt-SectEx2----
%@ Database source: Mathematics
Να μετατραπούν σε μοίρες οι παρακάτω γωνίες.
\begin{multicols}{3}
\begin{alist}
\item $ \frac{\pi}{4} $
\item $ \frac{2\pi}{3} $
\item $ \frac{\pi}{6} $
\item $ \frac{3\pi}{4} $
\item $ \frac{2\pi}{5} $
\item $ \pi $
\item $ \frac{3\pi}{2} $
\item $ \frac{4\pi}{5} $
\item $ 24\pi $
\item $ \frac{35\pi}{3} $
\item $ \frac{105\pi}{4} $
\item $ 400\pi $
\end{alist}
\end{multicols}
%# End of file Alg-TrigArithmoi-MoirAkt-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικοί αριθμοί/Ασκήσεις/Μετατροπή μοιρών σε ακτίνια και αντίστροφα/Alg-TrigArithmoi-MoirAkt-SectEx1.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-TrigArithmoi-MoirAkt-SectEx1----
%@ Database source: Mathematics
Να μετατραπούν σε ακτίνια (\en{rad}) οι παρακάτω γωνίες.
\begin{multicols}{3}
\begin{alist}
\item $ 30\degree $
\item $ 60\degree $
\item $ 45\degree $
\item $ 120\degree $
\item $ 150\degree $
\item $ 300\degree $
\item $ 270\degree $
\item $ 240\degree $
\item $ 330\degree $
\item $ 400\degree $
\item $ 480\degree $
\item $ 1200\degree $
\end{alist}
\end{multicols}
%# End of file Alg-TrigArithmoi-MoirAkt-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικοί αριθμοί/Ασκήσεις/Προβλήματα/Alg-TrigArithmoi-Problhmata-SectEx2.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-TrigArithmoi-Problhmata-SectEx2----
%@ Database source: Mathematics
Σε ένα αναλογικό ρολόι τοίχου δείκτης των ωρών έχει μήκος $ 7cm $, ο δείκτης των λεπτών έχει μήκος $ 12cm $ ενώ ο δείκτης των δευτερολέπτων έχει μήκος $ 10cm $.
\begin{center}
\begin{tikzpicture}[scale=.8]
\roloi[black]{3}{35}{55}
\end{tikzpicture}
\end{center}
\begin{rlist}
\item Να υπολογίσετε το μήκος του τόξου που διαγράφει η άκρη του δείκτη των δευτερολέπτων σε $ 20 $ δευτερόλεπτα.
\item Να υπολογίσετε το μέτρο και το μήκος του τόξου που διαγράφει η άκρη του ωροδείκτη σε $ 2 $ ώρες και $ 10 $ λεπτά.
\item Να υπολογίσετε το μήκος του τόξου που διαγράφει ο λεπτοδείκτης από τις $ 3:30 $μμ μέχρι τις $ 5:10 $μμ. Ποιο είναι το μέτρο του τόξου αυτού και ποιοι είναι οι τριγωνομετρικοί αριθμοί του;
\end{rlist}
%# End of file Alg-TrigArithmoi-Problhmata-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Τριγωνομετρικοί αριθμοί/Ασκήσεις/Προβλήματα/Alg-TrigArithmoi-Problhmata-SectEx1.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-TrigArithmoi-Problhmata-SectEx1----
%@ Database source: Mathematics
Ένα κτήριο ύψους $ h $ δημιουργεί σκιά στο έδαφος μήκους $ 250m $. Αν γνωρίζουμε ότι η γωνία που σχηματίζουν οι ακτίνες του ήλιου με το έδαφος είναι $ 30\degree $ τότε να βρεθεί το ύψος του κτηρίου.
%# End of file Alg-TrigArithmoi-Problhmata-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Παραμετρικές/Alg-GrammikaSys-Parametrikes-SectEx2.tex','17/7/2020 00:33','YES','','%# Database File : Alg-GrammikaSys-Parametrikes-SectEx2----
%@ Database source: Mathematics
Να βρεθούν οι τιμές της παραμέτρου $ \lambda\in\mathbb{R} $ ώστε η ευθεία $ (\lambda^2-1)x+(1-\lambda)y=2 $ να διέρχεται από το σημείο $ A(1,3) $.
%# End of file Alg-GrammikaSys-Parametrikes-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Παραμετρικές/Alg-GrammikaSys-Parametrikes-SectEx1.tex','17/7/2020 00:32','YES','','%# Database File : Alg-GrammikaSys-Parametrikes-SectEx1----
%@ Database source: Mathematics
Να βρεθούν οι τιμές της παραμέτρου $ \lambda\in\mathbb{R} $ ώστε η ευθεία $ \lambda x+(\lambda-1)y=4 $ να διέρχεται από το σημείο $ A(-2,3) $.
%# End of file Alg-GrammikaSys-Parametrikes-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Παραμετρικές/Alg-GrammikaSys-Parametrikes-SectEx3.tex','17/7/2020 00:34','YES','','%# Database File : Alg-GrammikaSys-Parametrikes-SectEx3----
%@ Database source: Mathematics
Αν γνωρίζουμε ότι το σημείο $ A(3\lambda-1,4-\lambda) $ ανήκει στην ευθεία $ 2x+3y=1 $ τότε να βρεθούν οι τιμές της παραμέτρου $ \lambda\in\mathbb{R} $.
%# End of file Alg-GrammikaSys-Parametrikes-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAnt-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντικατάστασης/Alg-GrammikaSys-MethAnt-SectEx5.tex','17/7/2020 00:39','YES','','%# Database File : Alg-GrammikaSys-MethAnt-SectEx5----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο της αντικατάστασης.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{-x+y=2,2x-2y=3} $
\item $ \systeme{x=2y-1,4x-8y=5} $
\item $ \systeme{2x+y=1,y=7-2x} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethAnt-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAnt-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντικατάστασης/Alg-GrammikaSys-MethAnt-SectEx4.tex','17/7/2020 00:38','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethAnt-SectEx4----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο της αντικατάστασης.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{x-2y=4,2x-4y=8} $
\item $ \systeme{3x-4y=1,-6x+8y=-2} $
\item $ \systeme{4x+2y=6,6x+3y=9} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethAnt-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAnt-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντικατάστασης/Alg-GrammikaSys-MethAnt-SectEx3.tex','17/7/2020 00:38','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethAnt-SectEx3----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο της αντικατάστασης.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{3x+2y=5,2x-y=1} $
\item $ \systeme{x+4y=-2,3x-7y=13} $
\item $ \systeme{4x-3y=-1,5x-2y=4} $
\item $ \systeme{7x+2y=29,3x-y=18} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethAnt-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAnt-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντικατάστασης/Alg-GrammikaSys-MethAnt-SectEx1.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethAnt-SectEx1----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω συστήματα με τη μέθοδο της αντικατάστασης.
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.,itemsep=0mm]
\item $\ccases{
\;x+2y=3\\
\;4x-3y=1}$
\item $\ccases{
\;3x+2y=5\\
\;-x+y=-5}$
\item $\ccases{
\;3x-y=9\\
\;-4x+2y=-10}$
\item $\ccases{
\;-2x+y=5\\
\;x+y=8}$
\end{enumerate}
\end{multicols}
%# End of file Alg-GrammikaSys-MethAnt-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAnt-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντικατάστασης/Alg-GrammikaSys-MethAnt-SectEx2.tex','17/7/2020 00:37','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethAnt-SectEx2----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο της αντικατάστασης.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{x-y=1,x=4} $
\item $ \systeme{2x+4y=8,y=3} $
\item $ \systeme{x=4,x-y=9} $
\item $ \systeme{x-y=2,x+y=8} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethAnt-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethOriz-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος οριζουσών/Alg-GrammikaSys-MethOriz-SectEx2.tex','17/7/2020 00:46','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethOriz-SectEx2----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο των οριζουσών.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{2x+y=5,4x+2y=3} $
\item $ \systeme{4x+2y=8,6x+3y=12} $
\item $ \systeme{x-y=3,2x-2y=5} $
\item $ \systeme{3x+5y=2,6x+10y=4} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethOriz-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethOriz-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος οριζουσών/Alg-GrammikaSys-MethOriz-SectEx1.tex','17/7/2020 00:45','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethOriz-SectEx1----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο των οριζουσών.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{2x+y=5,x-4y=-2} $
\item $ \systeme{3x+5y=16,4x-y=6} $
\item $ \systeme{x+5y=12,7x+3y=20} $
\item $ \systeme{6x-y=20,4x+9y=-6} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethOriz-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx4.tex','18/7/2020 00:36','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx4----
%@ Database source: Mathematics
Ένας πατέρας είναι 32 χρόνια μεγαλύτερος από το γιο του. Σε 8 χρόνια ο πατέρας θα έχει τα 3πλάσια χρόνια από το γιο του. Ποια είναι η ηλικία του πατέρα και του γιου?
%# End of file Alg-GrammikaSys-Problhmata-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx7','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx7.tex','18/7/2020 00:39','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx7----
%@ Database source: Mathematics
Σε ένα ορθογώνιο, το μήκος είναι διπλάσιο του πλάτους ενώ η περίμετρος είναι ίση με το μήκος αυξημένο κατά 12 μέτρα. Να βρεθούν οι πλευρές του ορθογωνίου.
%# End of file Alg-GrammikaSys-Problhmata-SectEx7','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx2.tex','18/7/2020 00:34','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx2----
%@ Database source: Mathematics
Ένας μαθητής έχει στο πορτοφόλι του 15 χαρτονομίσματα. Κάποια είναι των 5\officialeuro\; και κάποια των 10\officialeuro. Με τα χρήματα αυτά αγοράζει ένα κινητό τηλέφωνο αξίας 112\officialeuro\; και παίρνει ρέστα 8\officialeuro. Πόσα χαρτονομίσματα είναι των 5\officialeuro\; και πόσα των 10\officialeuro;
%# End of file Alg-GrammikaSys-Problhmata-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx3.tex','18/7/2020 00:35','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx3----
%@ Database source: Mathematics
Μια εταιρία κινητής τηλεφωνίας έχει τις εξής χρεώσεις : 0{,}07\officialeuro/sms και 0{,}09\officialeuro/1'' ομιλίας. Ένας συνδρομητής, με μια κάρτα των 10\officialeuro\;ξόδεψε συνολικά 120 λεπτά και μηνύματα. Πόσα ήταν τα λεπτά ομιλίας και πόσα τα μηνύματα;
%# End of file Alg-GrammikaSys-Problhmata-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx8','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx8.tex','18/7/2020 00:40','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx8----
%@ Database source: Mathematics
Η περίμετρος του τριγώνου του παρακάτω σχήματος είναι $ 38 $ εκατοστά. Να βρεθούν πραγματικοί οι αριθμοί $ x, y\in\mathbb{R} $ ώστε το τρίγωνο να είναι ισοσκελές.
\vspace{-5mm}
\begin{center}
\begin{tikzpicture}
\tkzDefPoint(3,0){C}
\tkzDefPoint(0,0){B}
\tkzDefPoint(1.5,1){A}
\tkzDrawPolygon[pl](A,B,C)
\tkzText(.2,.6){{\scriptsize $ 2x+6 $}}
\tkzText(3.2,.6){{\scriptsize $ 3x+2y-4 $}}
\tkzText(1.5,-.24){{\scriptsize $ y+7 $}}
\tkzLabelPoint[above](A){$A$}
\tkzLabelPoint[left](B){$B$}
\tkzLabelPoint[right](C){$\varGamma$}
\tkzDrawPoints(A,B,C)
\end{tikzpicture}
\end{center}
%# End of file Alg-GrammikaSys-Problhmata-SectEx8','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx6','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx6.tex','18/7/2020 00:37','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx6----
%@ Database source: Mathematics
Σε μια φάρμα ζουν 80 σε πλήθος κότες και αγελάδες. Αν όλα τα ζώα έχουν συνολικά 260 πόδια να βρεθούν πόσες κότες και πόσες αγελάδες ζουν στη φάρμα.
%# End of file Alg-GrammikaSys-Problhmata-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx1.tex','18/7/2020 00:33','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx1----
%@ Database source: Mathematics
Ένα ξενοδοχείο έχει 45 δωμάτια, άλλα δίκλινα και άλλα τρίκλινα. Συνολικά τα κρεβάτια είναι 110. Πόσα είναι τα δίκλινα και πόσα τα τρίκλινα δωμάτια;
%# End of file Alg-GrammikaSys-Problhmata-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Problhmata-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/Alg-GrammikaSys-Problhmata-SectEx5.tex','18/7/2020 00:37','ΟΧΙ','','%# Database File : Alg-GrammikaSys-Problhmata-SectEx5----
%@ Database source: Mathematics
Σε ένα κουτί υπάρχουν κόκκινες και πράσινες μπάλες. Αν προσθέσουμε στο κουτί 3 κόκκινες μπάλες, οι κόκκινες θα είναι διπλάσιες από τις πράσινες ενώ αν προσθέσουμε 4 πράσινες τότε, κόκκινες και πράσινες θα είναι ίσες. Πόσες μπάλες από το κάθε χρώμα υπάρχουν;
%# End of file Alg-GrammikaSys-Problhmata-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-GramExis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Γραμμική εξίσωση/Alg-GrammikaSys-GramExis-SectEx1.tex','02/10/2020 10:57','YES','','%# Database File : Alg-GrammikaSys-GramExis-SectEx1----
%@ Database source: Mathematics
Να εξεταστεί αν το σημείο $ A(2,1) $ ανήκει σε καθεμία από τις παρακάτω ευθείες.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ x-3y=4 $
\item $ 2x+3y=7 $
\item $ 4x+2y=5 $
\item $ 8x-7y=9 $
\item $ y=3 $
\item $ x=2 $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-GramExis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-GramExis-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Γραμμική εξίσωση/Alg-GrammikaSys-GramExis-SectEx3.tex','17/7/2020 00:29','ΟΧΙ','','%# Database File : Alg-GrammikaSys-GramExis-SectEx3----
%@ Database source: Mathematics
Να βρεθούν τα σημεία τομής των παρακάτω ευθειών με τους άξονες $ x''x $ και $ y''y $.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ x-2y=4 $
\item $ 4x-y=8 $
\item $ 2x-3y=-6 $
\item $ 7x-4y=11 $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-GramExis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-GramExis-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Γραμμική εξίσωση/Alg-GrammikaSys-GramExis-SectEx4.tex','17/7/2020 00:30','ΟΧΙ','','%# Database File : Alg-GrammikaSys-GramExis-SectEx4----
%@ Database source: Mathematics
Να βρεθούν τα σημεία τομής των παρακάτω ευθειών με τους άξονες $ x''x $ και $ y''y $.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ x=3 $
\item $ y=5 $
\item $ -2x=-7 $
\item $ 2y=4 $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-GramExis-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-GramExis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Γραμμική εξίσωση/Alg-GrammikaSys-GramExis-SectEx2.tex','17/7/2020 00:28','ΟΧΙ','','%# Database File : Alg-GrammikaSys-GramExis-SectEx2----
%@ Database source: Mathematics
Να βρεθεί ποιο ή ποια από τα παρακάτω σημεία ανήκουν στην ευθεία $ x+4y=9 $.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ A(2,-3) $
\item $ A(1,2) $
\item $ A(-3,3) $
\item $ A(0,2) $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-GramExis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Γραφική επίλυση συστήματος/Alg-GrammikaSys-GrafEpil-SectEx1.tex','17/7/2020 00:50','ΟΧΙ','','%# Database File : Alg-GrammikaSys-GrafEpil-SectEx1----
%@ Database source: Mathematics
Να λυθούν γραφικά τα παρακάτω γραμμικά συστήματα.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{x-y=3,3x+y=13} $
\item $ \systeme{2x+y=4,x+4y=8} $
\item $ \systeme{3x-y=2,6x-2y=4} $
\item $ \systeme{x-2y=-3,-2x+4y=5} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-GrafEpil-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Γραφική επίλυση συστήματος/Alg-GrammikaSys-GrafEpil-SectEx2.tex','17/7/2020 00:52','ΟΧΙ','','%# Database File : Alg-GrammikaSys-GrafEpil-SectEx2----
%@ Database source: Mathematics
Να βρεθούν, αν υπάρχουν, τα κοινά σημεία των παρακάτω ευθειών.
\begin{rlist}
\item $ x+3y=6 $ και $ 2x+y=8 $
\item $ 3x+4y=5 $ και $ -x+5y=3 $
\item $ 2x-y=10 $ και $ 4x-2y=7 $
\item $ 3x-y=2 $ και $ 6x-2y=4 $
\end{rlist}
%# End of file Alg-GrammikaSys-GrafEpil-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAntS-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντίθετων συντελεστών/Alg-GrammikaSys-MethAntS-SectEx2.tex','17/7/2020 00:43','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethAntS-SectEx2----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο των αντίθετων συντελεστών.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{4x-5y=-1,3x+7y=10} $
\item $ \systeme{4x-y=7,x+2y=4} $
\item $ \systeme{11x-8y=27,5x+9y=-13} $
\item $ \systeme{8x+6y=28,7x-5y=4} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethAntS-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAntS-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντίθετων συντελεστών/Alg-GrammikaSys-MethAntS-SectEx1.tex','17/7/2020 00:41','ΟΧΙ','','%# Database File : Alg-GrammikaSys-MethAntS-SectEx1----
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο των αντίθετων συντελεστών.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{x-y=3,x+y=7} $
\item $ \systeme{2x-3y=1,4x-5y=1} $
\item $ \systeme{x+y=10,3x+y=16} $
\item $ \systeme{-x-y=4,7x+4y=-19} $
\end{rlist}
\end{multicols}
%# End of file Alg-GrammikaSys-MethAntS-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-ExisAnt-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση εξίσωσης με αντικατάσταση/Alg-Exis2ou-ExisAnt-SectEx2.tex','23/7/2020 14:09','ΟΧΙ','','%# Database File : Alg-Exis2ou-ExisAnt-SectEx2----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=3mm]
\item $ x^4-5x^2+6=0 $
\item $ x^4-4x^2+3=0 $
\item $ x^4-6x^2+9=0 $
\item $ x^6-2x^3-15=0 $
\item $ 2x^4-x^2-10=0 $
\item $ x^8-10x^4+9=0 $
\end{alist}
\end{multicols}
%# End of file Alg-Exis2ou-ExisAnt-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-ExisAnt-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση εξίσωσης με αντικατάσταση/Alg-Exis2ou-ExisAnt-SectEx3.tex','23/7/2020 14:11','ΟΧΙ','','%# Database File : Alg-Exis2ou-ExisAnt-SectEx3----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{alist}
\item $ \left(x-1\right)^2-5\left|x-1\right|+6=0 $
\item $ \left(2x-3\right)^2-7\left|2x-3\right|+12=0 $
\item $ \left(x-2\right)^4-13\left(x-2\right)^2+36=0 $
\item $ \left(x+3\right)^6+19\left(x+3\right)^3-216=0 $
\end{alist}
%# End of file Alg-Exis2ou-ExisAnt-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-ExisAnt-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση εξίσωσης με αντικατάσταση/Alg-Exis2ou-ExisAnt-SectEx4.tex','23/7/2020 14:12','ΟΧΙ','','%# Database File : Alg-Exis2ou-ExisAnt-SectEx4----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{alist}
\item $ \left(x^2-x\right)^2+4\left|x^2-x\right|-12=0 $
\item $ \left(x+\frac{1}{x}\right)^2-7\left(x+\frac{1}{x}\right)+10=0 $
\item $ \left(x^3-2\right)^2+19\left(x^3-2\right)-150=0 $
\item $ \left(|x|-3\right)^2+8\left(|x|-3\right)-12=0 $
\end{alist}
%# End of file Alg-Exis2ou-ExisAnt-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-ExisAnt-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση εξίσωσης με αντικατάσταση/Alg-Exis2ou-ExisAnt-SectEx1.tex','23/7/2020 14:08','ΟΧΙ','','%# Database File : Alg-Exis2ou-ExisAnt-SectEx1----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=3mm]
\item $ x^2-5\left|x\right|+6=0 $
\item $ x^2-4\left|x\right|+3=0 $
\item $ x^2-2\left|x\right|-3=0 $
\item $ x^2+7\left|x\right|+10=0 $
\item $ 2x^2-\left|x\right|-10=0 $
\item $ x^2-10\left|x\right|+25=0 $
\end{alist}
\end{multicols}
%# End of file Alg-Exis2ou-ExisAnt-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-Parametrikes-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Παραμετρικές/Alg-Exis2ou-Parametrikes-SectEx1.tex','23/7/2020 14:33','ΟΧΙ','','%# Database File : Alg-Exis2ou-Parametrikes-SectEx1----
%@ Database source: Mathematics
Να βρεθούν οι σταθερές $ a,\beta\in\mathbb{R}^* $ έτσι ώστε η εξίσωση
\[ ax^2+(2a-3\beta)x+(a-\beta+2)=0 \] 
να έχει λύσεις τις $ x_1=-2, x_2=1 $.
%# End of file Alg-Exis2ou-Parametrikes-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-Parametrikes-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Παραμετρικές/Alg-Exis2ou-Parametrikes-SectEx3.tex','23/7/2020 14:34','ΟΧΙ','','%# Database File : Alg-Exis2ou-Parametrikes-SectEx3----
%@ Database source: Mathematics
Να βρεθούν οι σταθερές $ a, \beta\in\mathbb{R} $ έτσι ώστε η εξίσωση
 \[ x^2+(a+3\beta-2)x+4a-2a\beta-2=0 \]
να έχει λύσεις τις $ x_1=4-2a $ και $ x_2=\beta-3 $.
%# End of file Alg-Exis2ou-Parametrikes-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-Parametrikes-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Παραμετρικές/Alg-Exis2ou-Parametrikes-SectEx2.tex','23/7/2020 14:33','ΟΧΙ','','%# Database File : Alg-Exis2ou-Parametrikes-SectEx2----
%@ Database source: Mathematics
Να βρεθούν οι σταθερές $ a, \beta\in\mathbb{R} $ έτσι ώστε η εξίσωση
 \[ x^2+2(\beta-1)x+a+\beta^2-7=0 \] 
 να έχει μια διπλή λύση τη $ x=-2$.
%# End of file Alg-Exis2ou-Parametrikes-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-Problhmata-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Προβλήματα/Alg-Exis2ou-Problhmata-SectEx1.tex','23/7/2020 14:35','ΟΧΙ','','%# Database File : Alg-Exis2ou-Problhmata-SectEx1----
%@ Database source: Mathematics
Να βρεθεί η τιμή της μεταβλητής $ x $ για την οποία το εμβαδόν του παρακάτω σχήματος ισούται με $ E=84m^2 $.
\begin{center}
\begin{tikzpicture}
\draw  (-2.5,1.5) rectangle (1,-0.5);
\node at (-.75,1.75) {$x$};
\node at (-3.2,0.5) {$x-5$};
\node at (1.5,0.5) {};
\node at (-.75,0.5) {$E=84m^2$};
\end{tikzpicture}
\end{center}
%# End of file Alg-Exis2ou-Problhmata-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EpilExis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis2ou-EpilExis-SectEx2.tex','23/7/2020 12:57','ΟΧΙ','','%# Database File : Alg-Exis2ou-EpilExis-SectEx2----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=1mm]
\item $ x^2-4x+4=0 $
\item $ x^2-6x+9=0 $
\item $ x^2-10x+25=0 $
\item $ 4x^2+4x+1=0 $
\item $ 36x^2+12x+1=0 $
\item $ 4x^2+12x+9=0 $
\end{alist}
\end{multicols}
%# End of file Alg-Exis2ou-EpilExis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EpilExis-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis2ou-EpilExis-SectEx3.tex','03/12/2020 01:50','ΟΧΙ','','%# Database File : Alg-Exis2ou-EpilExis-SectEx3----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}
\item $ x^2+x+4=0 $
\item $ x^2+3x+12=0 $
\item $ 2x^2-3x+8=0 $
\item $ 2x^2+x+5=0 $
\end{alist}
\end{multicols}
%# End of file Alg-Exis2ou-EpilExis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EpilExis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis2ou-EpilExis-SectEx1.tex','22/7/2020 23:59','ΟΧΙ','','%# Database File : Alg-Exis2ou-EpilExis-SectEx1----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{multicols}{2}
\begin{alist}[leftmargin=4mm]
\item $ x^2-5x+6=0 $
\item $ x^2-3x+2=0 $
\item $ x^2-7x+12=0 $
\item $ x^2+3x-4=0 $
\item $ x^2-6x+8=0 $
\item $ x^2-6x+5=0 $
\item $ 2x^2-5x+3=0 $
\item $ 2x^2-9x+10=0 $
\item $ 3x^2-x-4=0 $
\end{alist}
\end{multicols}
%# End of file Alg-Exis2ou-EpilExis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EpilExis-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis2ou-EpilExis-SectEx4.tex','23/7/2020 12:58','ΟΧΙ','','%# Database File : Alg-Exis2ou-EpilExis-SectEx4----
%@ Database source: Mathematics
Να λυθούν οι παρακάτω εξισώσεις.
\begin{alist}[itemsep=0mm]
\item $ x^2-\left(\sqrt{2}-1\right)x-\sqrt{2}=0 $
\item $ x^2-\left(\sqrt{3}+1\right)x+\sqrt{3}=0 $
\item $ x^2+\left(\sqrt{3}+\sqrt{5}\right)x+\sqrt{15}=0 $
\item $ x^2-\left(\sqrt{8}-\sqrt{2}\right)x-4=0 $
\end{alist}
%# End of file Alg-Exis2ou-EpilExis-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-SwstoLathos-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Σωστό - Λάθος/Alg-Exis2ou-SwstoLathos-SectEx1.tex','23/7/2020 00:03','ΟΧΙ','','%# Database File : Alg-Exis2ou-SwstoLathos-SectEx1----
%@ Database source: Mathematics
Να χαρακτηριστούν οι παρακάτω προτάσεις ως σωστές (Σ) ή λανθασμένες (Λ).
\begin{alist}[leftmargin=4mm]
\item Αν για μια εξίσωση 2\textsuperscript{ου} βαθμού έχουμε $ \varDelta>0 $ τότε έχει 2 άνισες λύσεις.
\item Αν για μια εξίσωση 2\textsuperscript{ου} βαθμού έχουμε $ \varDelta<0 $ τότε έχει μια διπλή λύση.
\item Η εξίσωση $ ax^2+\beta x+\gamma=0 $ παριστάνει μια εξίσωση 2\textsuperscript{ου} βαθμού για κάθε τιμή του $ a $.
\item Αν $ x_1, x_2 $ είναι οι λύσεις μιας εξίσωσης 2\textsuperscript{ου} βαθμού τότε : $ x_1+x_2=\frac{\beta}{a} $ και $ x_1\cdot x_2=\frac{\gamma}{a} $.
\item Αν $ x_1, x_2 $ είναι οι λύσεις μιας εξίσωσης 2\textsuperscript{ου} βαθμού με $ x_1=-x_2 $ τότε $ \beta=0 $.
\end{alist}
%# End of file Alg-Exis2ou-SwstoLathos-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Exis2ou-EidhRizwn-SectEx5.tex','23/7/2020 14:32','ΟΧΙ','','%# Database File : Alg-Exis2ou-EidhRizwn-SectEx5----
%@ Database source: Mathematics
Να δειχτεί ότι η εξίσωση
\[ x^2+(a-3) x+a^2+4=0 \]
δεν έχει λύσεις για καμία τιμή του $ a $.
%# End of file Alg-Exis2ou-EidhRizwn-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Exis2ou-EidhRizwn-SectEx1.tex','23/7/2020 14:30','ΟΧΙ','','%# Database File : Alg-Exis2ou-EidhRizwn-SectEx1----
%@ Database source: Mathematics
Δίνεται η παρακάτω εξίσωση 2\tss{ου} βαθμού
\[ x^2+(\lambda-2) x+2\lambda^2=0 \]
όπου $ \lambda\in\mathbb{R} $ είναι μια τυχαία παράμετρος.
\begin{alist}
\item Να βρεθεί η διακρίνουσα της εξίσωσης.
\item Να βρεθούν οι τιμές τις παραμέτρου $ \lambda $ ώστε η εξίσωση να έχει δύο άνισες λύσεις.
\item Να βρεθούν οι τιμές τις παραμέτρου $ \lambda $ ώστε η εξίσωση να έχει μια διπλή λύση.
\item Για ποίες τιμές τις παραμέτρου $ \lambda $ είναι αδύνατη η εξίσωση;
\end{alist}
%# End of file Alg-Exis2ou-EidhRizwn-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Exis2ou-EidhRizwn-SectEx4.tex','23/7/2020 14:32','ΟΧΙ','','%# Database File : Alg-Exis2ou-EidhRizwn-SectEx4----
%@ Database source: Mathematics
Να δειχτεί ότι η εξίσωση
\[ ax^2+(a-1)x-1=0 \]
\begin{alist}
\item έχει λύσεις για κάθε $ a\in\mathbb{R}^* $.
\item έχει μια διπλή λύση για $ a=-1 $.
\end{alist}
%# End of file Alg-Exis2ou-EidhRizwn-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx6','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Exis2ou-EidhRizwn-SectEx6.tex','23/7/2020 14:32','ΟΧΙ','','%# Database File : Alg-Exis2ou-EidhRizwn-SectEx6----
%@ Database source: Mathematics
Να βρεθεί η τιμή της παραμέτρου $ a\in\mathbb{R}^* $ έτσι ώστε η εξίσωση
\[ 2ax^2+(a-4)x+a+2=0 \]
να έχει μια διπλή ρίζα.
%# End of file Alg-Exis2ou-EidhRizwn-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Exis2ou-EidhRizwn-SectEx2.tex','23/7/2020 14:30','ΟΧΙ','','%# Database File : Alg-Exis2ou-EidhRizwn-SectEx2----
%@ Database source: Mathematics
Δίνεται η παρακάτω εξίσωση 2\tss{ου} βαθμού
\[ x^2+3\lambda x+2\lambda^2-\lambda=0 \]
όπου $ \lambda\in\mathbb{R} $ είναι μια τυχαία παράμετρος.
\begin{alist}
\item Να βρεθεί η διακρίνουσα της εξίσωσης.
\item Να βρεθούν οι τιμές τις παραμέτρου $ \lambda $ ώστε η εξίσωση να έχει μια διπλή λύση.
\end{alist}
%# End of file Alg-Exis2ou-EidhRizwn-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Exis2ou-EidhRizwn-SectEx3.tex','23/7/2020 14:31','ΟΧΙ','','%# Database File : Alg-Exis2ou-EidhRizwn-SectEx3----
%@ Database source: Mathematics
Να δειχθεί ότι η εξίσωση
\[ x^2+x-\lambda^2=0 \]
έχει 2 άνισες λύσεις για κάθε τιμή του $ \lambda\in\mathbb{R} $.
%# End of file Alg-Exis2ou-EidhRizwn-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Τύποι του Vieta/Alg-Exis2ou-TypoiVieta-SectEx5.tex','23/7/2020 14:34','ΟΧΙ','','%# Database File : Alg-Exis2ou-TypoiVieta-SectEx5----
%@ Database source: Mathematics
Αν $ x_1,x_2 $ είναι οι λύσεις της εξίσωσης
\[ x^2-7x+8=0 \]
τότε χωρίς αυτή να λυθεί, να υπολογίσετε τις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{alist}
\item $ x_1+x_2 $
\item $ x_1x_2 $
\item $ x_1^2+x_2^2 $
\item $ x_1^2x_2+x_1x_2^2 $
\item $ x_1^3+x_2^3 $
\item $ \frac{1}{x_1}+\frac{1}{x_2} $
\end{alist}
\end{multicols}
%# End of file Alg-Exis2ou-TypoiVieta-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx6','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Τύποι του Vieta/Alg-Exis2ou-TypoiVieta-SectEx6.tex','23/7/2020 14:35','ΟΧΙ','','%# Database File : Alg-Exis2ou-TypoiVieta-SectEx6----
%@ Database source: Mathematics
Αν $ x_1,x_2 $ είναι οι λύσεις της εξίσωσης \[ x^2-4x+3=0 \] τότε να βρεθεί να βρεθεί η εξίσωση η οποία έχει λύσεις τις $ y_1=2x_1+x_2 $ και $ y_2=x_1-3x_2 $.
%# End of file Alg-Exis2ou-TypoiVieta-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Τύποι του Vieta/Alg-Exis2ou-TypoiVieta-SectEx1.tex','23/7/2020 12:59','ΟΧΙ','','%# Database File : Alg-Exis2ou-TypoiVieta-SectEx1----
%@ Database source: Mathematics
Να βρεθούν οι λύσεις $ x_1, x_2 $, μιας εξίσωσης 2\textsuperscript{ου} βαθμού, οι οποίες έχουν άθροισμα $ S $ και γινόμενο $ P $ με:
\begin{multicols}{2}
\begin{alist}[leftmargin=2mm]
\item $ S=9 $ και $ P=-10 $
\item $ S=-7 $ και $ P=12 $
\item $ S=6 $ και $ P=9 $
\item $ S=0 $ και $ P=4 $
\item $ S=12 $ και $ P=0 $
\item $ S=8 $ και $ P=-8 $
\end{alist}
\end{multicols}
%# End of file Alg-Exis2ou-TypoiVieta-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Τύποι του Vieta/Alg-Exis2ou-TypoiVieta-SectEx2.tex','23/7/2020 12:59','ΟΧΙ','','%# Database File : Alg-Exis2ou-TypoiVieta-SectEx2----
%@ Database source: Mathematics
Αν $ x_1, x_2 $ είναι οι λύσεις μιας εξίσωσης 2\tss{ου} βαθμού, να βρεθεί η εξίσωση, αν γι αυτήν ισχύει
\begin{alist}
\item $ x_1+x_2=7 $ και $ x_1\cdot x_2=6 $
\item $ x_1+x_2=8 $ και $ x_1\cdot x_2=12 $
\item $ x_1+x_2=-3 $ και $ x_1\cdot x_2=-28 $
\item $ x_1+x_2=-12 $ και $ x_1\cdot x_2=20 $
\end{alist}
%# End of file Alg-Exis2ou-TypoiVieta-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Τύποι του Vieta/Alg-Exis2ou-TypoiVieta-SectEx4.tex','23/7/2020 13:01','ΟΧΙ','','%# Database File : Alg-Exis2ou-TypoiVieta-SectEx4----
%@ Database source: Mathematics
Να βρεθούν οι λύσεις $ x_1, x_2 $, αν υπάρχουν, μιας εξίσωσης 2\tss{ου} βαθμού, για τις οποίες ισχύουν οι παρακάτω σχέσεις:
\begin{alist}
\item $ x_1+x_2=4 $ και $ x_1\cdot x_2=3 $
\item $ x_1+x_2=-7 $ και $ x_1\cdot x_2=-8 $
\item $ x_1+x_2=3 $ και $ x_1\cdot x_2=5 $
\item $ x_1^2+x_2^2=29 $ και $ (x_1+x_2)^2=49 $
\end{alist}
%# End of file Alg-Exis2ou-TypoiVieta-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Ασκήσεις/Τύποι του Vieta/Alg-Exis2ou-TypoiVieta-SectEx3.tex','23/7/2020 13:00','ΟΧΙ','','%# Database File : Alg-Exis2ou-TypoiVieta-SectEx3----
%@ Database source: Mathematics
Να βρεθεί η εξίσωση 2\tss{ου} βαθμού, η οποία έχει λύσεις τους παρακάτω αριθμούς $ x_1,x_2 $.
\begin{alist}
\item $ x_1=3 $ και $ x_2=5 $
\item $ x_1=-2 $ και $ x_2=-4 $
\item $ x_1=\frac{1}{2} $ και $ x_2=-\frac{3}{4} $
\item $ x_1=\sqrt{2} $ και $ x_2=3 $
\end{alist}
%# End of file Alg-Exis2ou-TypoiVieta-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-KoinLys-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Κοινές λύσεις ανισώσεων και διπλών ανισώσεων/Alg-Anis2ou-KoinLys-SectEx1.tex','08/10/2020 20:45','ΟΧΙ','','%# Database File : Alg-Anis2ou-KoinLys-SectEx1----
%@ Database source: Mathematics
Να βρεθούν οι κοινές λύσεις των παρακάτω ανισώσεων.
\begin{multicols}{2}
\begin{alist}
\item $ x^2-3x+2>0 $ και $ x^2-5x+4<0 $
\item $ x^2-x-2\leq0 $ και $ x^2-4x+3<0 $
\item $ x^2+4x+3\geq 0 $ και $ x^2-x-12>0 $
\item $ x^2-3x+2<0 $ και $ x^2+2x-3<0 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-KoinLys-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SectEx1.tex','21/8/2020 13:16','ΝΑΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SectEx1----
%@ Database source: Mathematics
Να βρεθούν οι λύσεις των παρακάτω ανισώσεων
\begin{multicols}{2}
\begin{alist}
\item $ x^2-3x+2>0 $
\item $ x^2-4x+3<0 $
\item $ 2x^2-5x+3\geq 0 $
\item $ x^2-x-2\leq 0 $
\item $ x^2-6x+5<0 $
\item $ 2x^2-x-1>0 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-EpilAnis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SectEx5.tex','21/8/2020 13:16','ΟΧΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SectEx5----
%@ Database source: Mathematics
Να βρεθούν οι λύσεις των παρακάτω ανισώσεων.
\begin{multicols}{2}
\begin{alist}
\item $ x^2+x+2>0 $
\item $ x^2-2x+3<0 $
\item $ x^2-4x+5\geq0 $
\item $ x^2+2x+2\leq0 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-EpilAnis-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SectEx3.tex','21/8/2020 13:16','ΝΑΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SectEx3----
%@ Database source: Mathematics
Να βρεθούν οι λύσεις των παρακάτω ανισώσεων
\begin{multicols}{2}
\begin{enumerate}
\item $ x^2-4x+4>0 $
\item $ x^2-6x+9\geq 0 $
\item $ x^2+10x+25<0 $
\item $ 4x^2+4x+1\leq 0 $
\item $ 4x^2+12x+9>0 $
\item $ x^2+x+\frac{1}{4}<0 $
\end{enumerate}
\end{multicols}
%# End of file Alg-Anis2ou-EpilAnis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SectEx4.tex','21/8/2020 13:16','ΟΧΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SectEx4----
%@ Database source: Mathematics
Να βρεθούν οι λύσεις των παρακάτω ανισώσεων
\begin{multicols}{2}
\begin{alist}
\item $ -x^2+2x-1>0 $
\item $ -4x^2+4x-1<0 $
\item $ -9x^2-6x-1\leq 0 $
\item $ -\frac{x^2}{4}+x-1\geq 0 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-EpilAnis-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SectEx2.tex','21/8/2020 13:16','ΝΑΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SectEx2----
%@ Database source: Mathematics
Να βρεθούν οι λύσεις των παρακάτω ανισώσεων
\begin{multicols}{2}
\begin{alist}
\item $ -x^2+7x-12>0 $
\item $ -2x^2+3x+2<0 $
\item $ -x^2+2x+15\geq 0 $
\item $ -3x^2-2x+1\leq 0 $
\item $ -x^2-3x>0 $
\item $ -4x^2+x<0 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-EpilAnis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Παραγοντοποίηση τριωνύμου/Alg-Anis2ou-ParagTriwn-SectEx2.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-Anis2ou-ParagTriwn-SectEx2----
%@ Database source: Mathematics
Να παραγοντοποιηθούν τα παρακάτω τριώνυμα:
\begin{multicols}{2}
\begin{alist}
\item $ x^2-4x+4 $
\item $ x^2+6x+9 $
\item $ 4x^2+4x+1 $
\item $ x^2-10x+25 $
\item $ x^2+x+\frac{1}{4} $
\item $ 3x^2-4x+1 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-ParagTriwn-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Παραγοντοποίηση τριωνύμου/Alg-Anis2ou-ParagTriwn-SectEx4.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-Anis2ou-ParagTriwn-SectEx4----
%@ Database source: Mathematics
Να απλοποιηθούν οι παρακάτω ρητές παραστάσεις:
\begin{multicols}{2}
\begin{alist}
\item $ \dfrac{x^2-3x+2}{x^2-x-2} $
\item $ \dfrac{x^2-4x+3}{x^2-7x+12} $
\item $ \dfrac{x^2+x-6}{x^2-2x-15} $
\item $ \dfrac{2x^2-3x+1}{2x^2-x-1} $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-ParagTriwn-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Παραγοντοποίηση τριωνύμου/Alg-Anis2ou-ParagTriwn-SectEx1.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-Anis2ou-ParagTriwn-SectEx1----
%@ Database source: Mathematics
Να παραγοντοποιηθούν τα παρακάτω τριώνυμα:
\begin{multicols}{2}
\begin{alist}
\item $ x^2-4x+3 $
\item $ x^2-x-2 $
\item $ 2x^2-5x+3 $
\item $ 4x^2+3x+1 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-ParagTriwn-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Παραγοντοποίηση τριωνύμου/Alg-Anis2ou-ParagTriwn-SectEx3.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-Anis2ou-ParagTriwn-SectEx3----
%@ Database source: Mathematics
Να παραγοντοποιηθούν τα παρακάτω τριώνυμα:
\begin{multicols}{2}
\begin{alist}
\item $ x^2-x-2 $
\item $ y^2-3y-4 $
\item $ z^2-2z-15 $
\item $ 4t^2-12t+9 $
\end{alist}
\end{multicols}
%# End of file Alg-Anis2ou-ParagTriwn-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Anis2ou-EidhRizwn-SectEx2.tex','14/10/2020 15:22','ΟΧΙ','','%# Database File : Alg-Anis2ou-EidhRizwn-SectEx2----
%@ Database source: Mathematics
Δίνεται η εξίσωση $ x^2-\left(\lambda^2-4\lambda+3\right)x+4-3\lambda-\lambda^2=0  $. Να βρεθούν οι τιμές της παραμέτρου $ \lambda $ ώστε
\begin{alist}
\item η εξίσωση να έχει δύο λύσεις άνισες.
\item η εξίσωση να έχει μια διπλή λύση.
\item οι ρίζες τις εξίσωσης να είναι
\begin{multicols}{4}
\begin{alist}
\item ομόσημες
\item ετερόσημες
\item θετικές
\item αρνητικές
\end{alist}
\end{multicols}
\end{alist}
%# End of file Alg-Anis2ou-EidhRizwn-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Ασκήσεις/Είδη και πλήθος ριζών τριωνύμου 2ου β./Alg-Anis2ou-EidhRizwn-SectEx1.tex','14/10/2020 15:18','ΟΧΙ','','%# Database File : Alg-Anis2ou-EidhRizwn-SectEx1----
%@ Database source: Mathematics
Να βρεθούν οι τιμές της παραμέτρου $ \lambda\in\mathbb{R} $ ώστε η εξίσωση 
\[ x^2+\left( \lambda^2-3\lambda+2\right) x+1=0 \]
\begin{rlist}
\item να έχει δύο λύσεις άνισες.
\item οι λύσεις της εξίσωσης να είναι θετικές για κάθε τιμή της παραμέτρου $ \lambda $.
\end{rlist}
%# End of file Alg-Anis2ou-EidhRizwn-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SolSE1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Λύσεις ασκήσεων ενότητας/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SolSE1.tex','09/7/2020 17:55','ΟΧΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SolSE1----
%@ Database source: Mathematics
\begin{alist}
\item Οι συντελεστές του τριωνύμου είναι $ a=1,\beta =-3 $  και $ \gamma=2 $. Η διακρίνουσα του θα ισούται με
\[ \varDelta=\beta^2-4a\gamma=(-3)^2-4\cdot1\cdot2=9-8=1>0 \]
επομένως έχει δύο άνισες ρίζες:
\[ x_{1,2}=\frac{-\beta\pm\sqrt{\varDelta}}{2a}=\frac{3\pm\sqrt{1}}{2\cdot1}=\frac{3\pm 1}{2} \]
άρα
\[ x_1=\frac{3+1}{2}=2\ \ \textrm{ή}\ \ x_2=\frac{3-1}{2}=1 \]
Τα πρόσημα του τριωνύμου φαίνονται στον παρακάτω πίνακα:
\begin{center}
\begin{tikzpicture}
\tikzset{t style/.style = {style = dashed}}
\tkzTabInit[color,lgt=3,espcl=2,colorC = \xrwma!40,
colorL = \xrwma!20,
colorV = \xrwma!40]%
{$x$ / .8,$x^2-3x+2$ /0.8}%
{$-\infty$,$1$,$2$,$+\infty$}%
\tkzTabLine{ , +, z, -, z, +, }
\end{tikzpicture}
\end{center}
\end{alist}
%# End of file Alg-Anis2ou-EpilAnis-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SolSE2','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Λύσεις ασκήσεων ενότητας/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SolSE2.tex','07/10/2020 17:19','ΟΧΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SolSE2----
%@ Database source: Mathematics
λυση 2
%# End of file Alg-Anis2ou-EpilAnis-SolSE2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis2ou-EpilAnis-SolSE3','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Λύσεις ασκήσεων ενότητας/Επίλυση απλής πολυωνυμικής ανίσωσης/Alg-Anis2ou-EpilAnis-SolSE3.tex','08/10/2020 01:33','ΟΧΙ','','%# Database File : Alg-Anis2ou-EpilAnis-SolSE3----
%@ Database source: Mathematics
lysh3
%# End of file Alg-Anis2ou-EpilAnis-SolSE3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-IdiotAnis-SectEx7','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων/Alg-Diataxh-IdiotAnis-SectEx7.tex','22/7/2020 23:35','ΟΧΙ','','%# Database File : Alg-Diataxh-IdiotAnis-SectEx7----
%@ Database source: Mathematics
Δίνονται δύο πραγματικοί αριθμοί $ a,\beta\in\mathbb{R} $ για τους οποίους ισχύουν οι σχέσεις $ 1<a<5 $ και $ 2<\beta<3 $. Να βρεθεί μεταξύ ποιών αριθμών βρίσκονται οι παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{alist}
\item $ 3a+2\beta $
\item $ 4a-3\beta $
\item $ 4a\beta-5 $
\item $ \frac{3a}{2\beta} $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-IdiotAnis-SectEx7','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-IdiotAnis-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων/Alg-Diataxh-IdiotAnis-SectEx5.tex','22/7/2020 23:33','ΟΧΙ','','%# Database File : Alg-Diataxh-IdiotAnis-SectEx5----
%@ Database source: Mathematics
Έστω $ a\in\mathbb{R} $ πραγματικός αριθμός για τον οποίο ισχύει $ 2<a<5 $. Να βρεθεί μεταξύ ποιών αριθμών βρίσκονται οι παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{alist}
\item $ a-7 $
\item $ 3a+4 $
\item $ 3-2a $
\item $ 7a+13 $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-IdiotAnis-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-IdiotAnis-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων/Alg-Diataxh-IdiotAnis-SectEx4.tex','22/7/2020 23:18','ΟΧΙ','','%# Database File : Alg-Diataxh-IdiotAnis-SectEx4----
Έστω ένας πραγματικός αριθμός $ a $ για τον οποίο γνωρίζουμε ότι ισχύει $ 2<a<4 $. Να βρεθούν οι αριθμοί μεταξύ των οποίων βρίσκονται οι παρακάτω παραστάσεις.
\begin{multicols}{3}
\begin{alist}
\item $ a^2-4 $
\item $ 2a^2+3 $
\item $ a^3 $
\item $ 2a-a^2 $
\item $ 3a^2-a^3 $
\item $ a^4-3a^2 $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-IdiotAnis-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-IdiotAnis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων/Alg-Diataxh-IdiotAnis-SectEx1.tex','22/7/2020 23:08','ΟΧΙ','','%# Database File : Alg-Diataxh-IdiotAnis-SectEx1----
Δίνεται πραγματικός αριθμός $ a\in\mathbb{R} $ τέτοιος ώστε να ισχύει $ 1<a<4 $. Να βρεθούν τα πρόσημα των παρακάτω παραστάσεων.
\begin{multicols}{3}
\begin{alist}
\item $ a-4 $
\item $ a-1 $
\item $ 1-a $
\item $ 8-a $
\item $ a-5 $
\item $ 7-a $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-IdiotAnis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-IdiotAnis-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων/Alg-Diataxh-IdiotAnis-SectEx3.tex','22/7/2020 23:28','ΟΧΙ','','%# Database File : Alg-Diataxh-IdiotAnis-SectEx3----
Δίνονται τρεις πραγματικοί αριθμοί $ x,y,z $ τέτοιοι ώστε $ x<y<z $. Να βρεθεί το πρόσημο των παρακάτω παραστάσεων.
\begin{alist}
\item $ (x-y)(z-x) $
\item $ (x-z)(y-x) $
\item $ (y-z)(x-y)(x-z) $
\end{alist}
%# End of file Alg-Diataxh-IdiotAnis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-IdiotAnis-SectEx6','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων/Alg-Diataxh-IdiotAnis-SectEx6.tex','22/7/2020 23:34','ΟΧΙ','','%# Database File : Alg-Diataxh-IdiotAnis-SectEx6----
Δίνονται δύο πραγματικοί αριθμοί $ a,\beta\in\mathbb{R} $ για τους οποίους ισχύουν οι σχέσεις $ 3<a<7 $ και $ 2<\beta<4 $. Να αποδειχθούν οι παρακάτω ανισότητες.
\begin{multicols}{2}
\begin{alist}
\item $ 8<2a+\beta<18 $
\item $ -1<a-\beta<5 $
\item $ 24<4a\beta<112 $
\item $ \frac{3}{4}<\frac{a}{\beta}<\frac{7}{2} $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-IdiotAnis-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-IdiotAnis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Ιδιότητες ανισοτήτων - Κατασκευή ανισοτήτων/Alg-Diataxh-IdiotAnis-SectEx2.tex','22/7/2020 23:27','ΟΧΙ','','%# Database File : Alg-Diataxh-IdiotAnis-SectEx2----
Έστω ένας πραγματικός αριθμός $ a\in\mathbb{R} $ ο οποίος ικανοποιεί τη σχέση $ 2<a<7 $. Να βρεθούν τα πρόσημα των παρακάτω παραστάσεων.
\begin{alist}
\item $ (a-2)(a-7) $
\item $ (2a-3)(7-a)(1-a) $
\item $ (3a-5)(14-2a) $
\end{alist}
%# End of file Alg-Diataxh-IdiotAnis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-SygkrAr-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Σύγκριση αριθμών/Alg-Diataxh-SygkrAr-SectEx4.tex','22/7/2020 23:30','ΟΧΙ','','%# Database File : Alg-Diataxh-SygkrAr-SectEx4----
Να συγκριθούν οι παρακάτω αλγεβρικές παραστάσεις $ A $ και $ B $ στις παρακάτω περιπτώσεις.
\begin{alist}
\item $ A=\frac{a-\beta}{a+\beta} $ και $ B=\frac{a\beta}{a^2-\beta^2} $
\item 
\item 
\item 
\end{alist}
%# End of file Alg-Diataxh-SygkrAr-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-SygkrAr-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Σύγκριση αριθμών/Alg-Diataxh-SygkrAr-SectEx3.tex','22/7/2020 23:30','ΟΧΙ','','%# Database File : Alg-Diataxh-SygkrAr-SectEx3----
Να συγκριθούν οι παρακάτω αλγεβρικές παραστάσεις $ A $ και $ B $ στις παρακάτω περιπτώσεις.
\begin{alist}
\item $ A=(x+y)^2 $ και $ B=1-(x-y)^2 $
\item 
\item 
\item 
\end{alist}
%# End of file Alg-Diataxh-SygkrAr-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-SygkrAr-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Σύγκριση αριθμών/Alg-Diataxh-SygkrAr-SectEx2.tex','22/7/2020 23:31','ΟΧΙ','','%# Database File : Alg-Diataxh-SygkrAr-SectEx2----
Να συγκρίνετε τους πραγματικούς αριθμούς $ A $ και $ B $ στις παρακάτω περιπτώσεις.
\begin{alist}
\item $ A=2^{70} $ και $ B=4^{30} $
\item $ A=9^{120} $ και $ B=8^{150} $
\item $ A= $ και $ B= $
\item $ A= $ και $ B= $
\end{alist}
%# End of file Alg-Diataxh-SygkrAr-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-SygkrAr-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Σύγκριση αριθμών/Alg-Diataxh-SygkrAr-SectEx1.tex','22/7/2020 23:31','ΟΧΙ','','%# Database File : Alg-Diataxh-SygkrAr-SectEx1----
Να συγκρίνετε τους πραγματικούς αριθμούς $ A $ και $ B $ στις παρακάτω περιπτώσεις.
\begin{alist}
\item $ A=3\sqrt{3}-1 $ και $ B=2\sqrt{3}-2 $
\item $ A=4\sqrt{5}+\sqrt{2} $ και $ B=3\sqrt{5}+2\sqrt{2} $
\item $ A=\frac{2\sqrt{2}-1}{2} $ και $ B=\frac{3}{2} $
\item $ A=\left(\!\sqrt{2}\right)^3+1 $ και $ B=\sqrt{2}+2 $
\end{alist}
%# End of file Alg-Diataxh-SygkrAr-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-ApodAnis-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Απόδειξη ανισότητας/Alg-Diataxh-ApodAnis-SectEx1.tex','22/7/2020 23:25','ΟΧΙ','','%# Database File : Alg-Diataxh-ApodAnis-SectEx1----
Να αποδειχθεί ότι ισχύουν οι παρακάτω ανισότητες για κάθε τιμή της μεταβλητής $ x $.
\begin{multicols}{2}
\begin{alist}
\item $ x^2+1\geq 2x $
\item $ 4x\leq 1+4x^2 $
\item $ 6x-9\leq x^2 $
\item $ (x+1)^2\geq 4x $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-ApodAnis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-ApodAnis-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Απόδειξη ανισότητας/Alg-Diataxh-ApodAnis-SectEx2.tex','22/7/2020 23:29','ΟΧΙ','','%# Database File : Alg-Diataxh-ApodAnis-SectEx2----
Να αποδειχθεί ότι ισχύουν οι παρακάτω ανισότητες για κάθε τιμή των μεταβλητών $ x,y $.
\begin{multicols}{2}
\begin{alist}
\item $ x^2+y^2\geq 2xy $
\item $ x^2+1\geq 2y-y^2 $
\item $ (x+y)^2\geq 4xy $
\item $ 4x^2\geq x^2-4y^2 $
\end{alist}
\end{multicols}
\vspace{-7mm}
\begin{alist}[start=5]
\item $ 3x^2+1\geq 4x-x^2-y^2 $
\end{alist}
%# End of file Alg-Diataxh-ApodAnis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-MidAthr-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Μηδενικό άθροισμα/Alg-Diataxh-MidAthr-SectEx1.tex','21/8/2020 16:14','ΟΧΙ','','%# Database File : Alg-Diataxh-MidAthr-SectEx1----
Να βρεθούν οι πραγματικοί αριθμοί $ x,y $ σε καθεμία από τις παρακάτω περιπτώσεις.
\begin{alist}
\item $ (x-1)^2+(y+2)^2=0 $
\item $ (3x-6)^2+(2y-8)^2=0 $
\item $ x^2+y^2-2y+1=0 $
\item $ x^2+y^2-4x+6y+13=0 $
\end{alist}
%# End of file Alg-Diataxh-MidAthr-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx4','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx4.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx4----
Να παραστήσετε γραφικά σε άξονα τα σύνολα αριθμών που ορίζονται από τις παρακάτω ανισότητες.
\begin{multicols}{3}
\begin{alist}[leftmargin=2mm]
\item $ -3\leq x\leq 2 $
\item $ 1<x\leq 5 $
\item $ 0<x<10 $
\item $ x>-4 $
\item $ x<0 $
\item $ x\geq 9 $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx2','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx2.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx2----
Να γραφτούν οι παρακάτω ανισότητες με τη μορφή διαστημάτων.
\begin{multicols}{2}
\begin{alist}
\item $ 2\leq x\leq 8 $
\item $ -3<x<10 $
\item $ x>-2 $
\item $ x\leq 4 $
\item $ -1\leq x<3 $
\item $ x\geq 0 $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx6','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx6.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx6----
Να γράψετε σε μορφή διαστήματος κάθε σύνολο αριθμών από τα παρακάτω, όπως αυτά φαίνονται στους άξονες.
\begin{multicols}{2}
\begin{alist}
\item \tikzitem\begin{tikzpicture}
\Xapeiro{2}{0}{2.3}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{a}{0}
\end{tikzpicture}
\item \tikzitem\begin{tikzpicture}
\ApeiroX{4}{2}{-.3}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{k}{2}
\end{tikzpicture}
\item \tikzitem\begin{tikzpicture}
\Xapeiro{-5}{0}{2.3}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{k}{0}
\end{tikzpicture}
\item \tikzitem\begin{tikzpicture}
\ApeiroX{0}{2}{-.3}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{a}{2}
\end{tikzpicture}
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx3','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx3.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx3----
Να παρασταθούν τα παρακάτω διαστήματα γραφικά, πάνω στην ευθεία των πραγματικών αριθμών.
\begin{multicols}{3}
\begin{alist}[leftmargin=5mm]
\item $ [-2,2] $
\item $ [4,9) $
\item $ (-3,0] $
\item $ (4,+\infty) $
\item $ (-\infty,3] $
\item $ (-4,5) $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx9','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx9.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx9----
Δίνονται τα διαστήματα πραγματικών αρθμών $ \varDelta_1=[1,4],\varDelta_2=(2,+\infty) $. Να βρεθούν τα παρακάτω σύνολα.
\begin{multicols}{3}
\begin{alist}
\item $ \varDelta_1\cup\varDelta_2 $
\item $ \varDelta_1\cap\varDelta_2 $
\item $ \varDelta_1'' $
\item $ \varDelta_2'' $
\item $ \varDelta_1-\varDelta_2 $
\item $ \varDelta_2-\varDelta_1 $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx9','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx5','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx5.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx5----
Να γράψετε σε μορφή διαστήματος κάθε σύνολο αριθμών από τα παρακάτω, όπως αυτά φαίνονται στους άξονες.
\begin{multicols}{2}
\begin{alist}
\item \tikzitem\begin{tikzpicture}
\Diasthma{1}{4}{0}{2}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{k}{0}
\Akro{k}{2}
\end{tikzpicture}
\item \tikzitem\begin{tikzpicture}
\Diasthma{-2}{5}{0}{2}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{a}{0}
\Akro{k}{2}
\end{tikzpicture}
\item \tikzitem\begin{tikzpicture}
\Diasthma{0}{1}{0}{2}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{k}{0}
\Akro{a}{2}
\end{tikzpicture}
\item \tikzitem\begin{tikzpicture}
\Diasthma{-4}{5}{0}{2}{.3}{red!80!black}
\Axonas{-.3}{2.5}
\Akro{a}{0}
\Akro{a}{2}
\end{tikzpicture}
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx1.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx1----
Να γραφτούν τα παρακάτω διαστήματα με τη μορφή ανισοτήτων.
\begin{multicols}{3}
\begin{alist}
\item $ [3,7] $
\item $ [-2,5) $
\item $ (0,3] $
\item $ (-\infty,4] $
\item $ (0,+\infty) $
\item $ (-4,4) $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx7','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx7.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx7----
Να βρεθούν τα κοινά στοιχεία των παρακάτω διαστημάτων.
\begin{multicols}{2}
\begin{alist}
\item $ [2,4]\ ,\ [3,7] $
\item $ (-\infty,5)\ ,\ (-3,7) $
\item $ [0,3)\ ,\ (1,3] $
\item $ (-\infty,4)\ ,\ [5,+\infty) $
\item $ (-3,4]\ ,\ (-\infty,4) $
\item $ [2,+\infty)\ ,\ (3,+\infty) $
\end{alist}
\end{multicols}
%# End of file Alg-Diataxh-Diasthmata-SectEx7','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Diataxh-Diasthmata-SectEx8','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διάταξη αριθμών/Ασκήσεις/Διαστήματα/Alg-Diataxh-Diasthmata-SectEx8.tex','14/10/2020 14:27','ΟΧΙ','','%# Database File : Alg-Diataxh-Diasthmata-SectEx8----
Αν $ \varDelta_1,\varDelta_2 $ είναι δύο διαστήματα πραγματικών αριθμών τότε να βρεθεί η ένωση $ \varDelta_1\cup\varDelta_2 $ και η τομή τους $ \varDelta_1\cap\varDelta_2 $ σε καθεμία από τις παρακάτω περιπτώσεις.
\begin{alist}
\item $ \varDelta_1=[-3,3] $ και $ \varDelta_2=(-2,4] $.
\item $ \varDelta_1=[1,4) $ και $ \varDelta_2=(0,+\infty) $.
\item $ \varDelta_1=(-4,0) $ και $ \varDelta_2=(-\infty,-2] $.
\item $ \varDelta_1=(-\infty,10) $ και $ \varDelta_2=(11,+\infty) $.
\end{alist}
%# End of file Alg-Diataxh-Diasthmata-SectEx8','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma
%@ Database source: Mathematics
Θα λέμε ότι μια συνάρτηση $ f $ είναι παραγωγίσιμη σε ένα ανοιχτό διάστημα $ (a,\beta) $ αν είναι παραγωγίσιμη σε κάθε σημείο του διαστήματος $ (a,\beta) $.
%# End of file Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma','Basic','PdfLaTeX',NULL,0,'Γ'' Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synarthsh','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synarthsh.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synarthsh
%@ Database source: Mathematics
Συνάρτηση ονομάζεται η διαδικασία (αντιστοίχηση) με την οποία κάθε στοιχείο ενός συνόλου $ A $ αντιστοιχεί σε \textbf{ένα μόνο} στοιχείο ενός συνόλου $ B $.\\Συμβολίζεται με οποιοδήποτε γράμμα του λατινικού ή του ελληνικού αλφαβήτου $ f, g, h, t, s, \sigma\ldots $ και είναι : \[ f:A\rightarrow B \]
Είναι η σχέση που συνδέει δύο μεταβλητές $ x,y $ όπου κάθε τιμή της πρώτης $ (x\in A) $, του πρώτου συνόλου, αντιστοιχεί σε μόνο μια τιμή της δεύτερης $ (y\in B) $, του δεύτερου συνόλου.\vspace{-3mm}
\begin{center}
\begin{figure}[h]
\centering
\begin{tikzpicture}[scale=.6]
\draw(0,0) ellipse (1cm and 1.5cm);
\draw(4,0) ellipse (1cm and 1.5cm);
\draw[fill=\xrwma!30] (4.1,0) ellipse (.6cm and 1.1cm);
\draw[-latex] (0,.2) arc (140:40:2.6);
\tkzDefPoint(0,.2){A}
\tkzDefPoint(4,.2){B}
\tkzDrawPoints(A,B)
\tkzLabelPoint[left](A){{\footnotesize $ x $}}
\tkzLabelPoint[right](B){{\footnotesize $ y $}}
\tkzText(0,1.8){$ A $}
\tkzText(4,1.8){$ B $}
\tkzText(2,1.45){$ f $}
\draw[-latex] (3.5,0) -- (2.7,-1) node[anchor=north east] {\footnotesize $ f\left( A\right)  $};
\end{tikzpicture}
\end{figure}
\end{center}
\vspace{-1.1cm}
\begin{itemize}[itemsep=0mm]
\item Η μεταβλητή $ x $ του συνόλου $ A $ ονομάζεται \textbf{ανεξάρτητη} ενώ η $ y $ \textbf{εξαρτημένη}.
\item Η τιμή της $ y $ ονομάζεται \textbf{τιμή} της $ f $ στο $ x $ και συμβολίζεται $ y=f(x) $.
\item Ο κανόνας της συνάρτησης, με τον οποίο γίνεται η αντιστοίχηση από το $ x $  στο $ f(x) $, εκφράζεται συμβολικά με τη βοήθεια του $ x $ και ονομάζεται \textbf{τύπος της συνάρτησης}.
\item Το σύνολο $ A $ λέγεται \textbf{πεδίο ορισμού} της συνάρτησης $ f $. Είναι το σύνολο των δυνατών τιμών την ανεξάρτητης μεταβλητής της συνάρτησης.
\item Το σύνολο με στοιχεία όλες τις δυνατές τιμές $ f(x) $ της εξαρτημένης μεταβλητής για κάθε $ x\in A $ λέγεται \textbf{σύνολο τιμών} της $ f $, συμβολίζεται $ f\left(A\right) $ και ισχύει $ f\left(A\right)\subseteq B $.
\item Μια συνάρτηση συμβολίζεται επίσης με τους εξής τρόπους : \[ x\overset{f}{\mapsto}f(x)\;\;,\;\;A\overset{f}{\rightarrow}f\left(A\right) \]
\item Για το συμβολισμό της ανεξάρτητης μεταβλητής ή της συνάρτησης μπορούμε να χρησιμοποιήσουμε οποιοδήποτε συμβολισμό στη θέση της μεταβλητής $ x $ ή του ονόματος $ f $ της συνάρτησης αντίστοιχα. \[ f(x)\;\;,\;\;g(t)\;\;,\;\;h(s)\ldots \]
\vspace{-3mm}
\item Για να ορίσουμε μια συνάρτηση θα πρέπει να γνωρίζουμε
\vspace{-3mm}
\begin{enumerate}[itemsep=0mm]
\begin{multicols}{2}
\item To πεδίο ορισμού $ A $.
\item Το σύνολο $ B $.
\end{multicols}
\vspace{-3mm}
\item Τον τύπο $ f(x) $ της συνάρτησης, για κάθε $ x\in A $.
\end{enumerate}
\item Εάν τα σύνολα $ A,B $ είναι υποσύνολα του συνόλου των πραγματικών αριθμών τότε μιλάμε για \textbf{πραγματική συνάρτηση πραγματικής μεταβλητής}.
%# End of file Analysh-Or-Synarthsh','Basic','PdfLaTeX',NULL,0,'Α'' Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Παράγωγος συνάρτηση/ΟρισμοίAnalysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma.tex','09/12/2021 12:08','-','@article{Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma,}','%# Database File : Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma
%@ Database source: Mathematics
Θα λέμε ότι μια συνάρτηση $ f $ είναι παραγωγίσιμη σε ένα κλειστό διάστημα $ [a,\beta] $ αν είναι παραγωγίσιμη σε κάθε σημείο του ανοιχτού διαστήματος $ (a,\beta) $ και επιπλέον
\[ \lim_{x\to a^+}{\frac{f(x)-f(a)}{x-a}}\in\mathbb{R}\ \ \textrm{και}\ \ \lim_{x\to \beta^-}{\frac{f(x)-f(\beta)}{x-\beta}}\in\mathbb{R} \]
%# End of file Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma','Basic','PdfLaTeX','Παραγωγίσιμη συνάρτηση σε κλειστό διάστημα',0,'Γ'' Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Grafikh_Parastash','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Grafikh_Parastash.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Grafikh_Parastash
%@ Database source: Mathematics
Γραφική παράσταση μιας συνάρτησης $ f:A\rightarrow\mathbb{R} $ ονομάζεται το σύνολο των σημείων του επιπέδου με συντεταγμένες $ M(x,y) $ όπου \[ y=f(x)\ \ \text{ για κάθε }x\in A \]
δηλαδή το σύνολο των σημείων $ M(x,f(x)) $, για κάθε $ x\in A $. Συμβολίζεται με $ C_f $ είναι 
\[ C_f=\{M(x,y)|y=f(x)\textrm{ για κάθε }x\in A\} \]
%# End of file Analysh-Or-Grafikh_Parastash','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Topika_Akrotata','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Topika_Akrotata.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Topika_Akrotata
%@ Database source: Mathematics
Το τοπικό μέγιστο και το τοπικό ελάχιστο μιας συνάρτησης $ f $ λέγονται \textbf{τοπικά ακρότατα}. 
%# End of file Analysh-Or-Topika_Akrotata','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Elaxisto','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Elaxisto.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Elaxisto
%@ Database source: Mathematics
Έστω μια συνάρτηση $ f $ με πεδίο ορισμού ένα σύνολο $ A $ και έστω $ x_0\in A $. Η $ f $ θα λέμε ότι παρουσιάζει ολικό ελάχιστο στη θέση $ x_0 $, το $ f(x_0) $ όταν 
\[ f(x)\geq f(x_0)\ \ \textrm{για κάθε}\ \ x\in A \]
Το $ x_0 $ λέγεται \textbf{θέση} του ελάχιστου.
%# End of file Analysh-Or-Elaxisto','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Deuterh-N-osth_Paragwgos','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Deuterh-N-osth_Paragwgos.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Deuterh-N-osth_Paragwgos
%@ Database source: Mathematics
Έστω $ A_1 $ το σύνολο όλων των σημείων του $ D_f $ της $ f $ για τα οποία η $ f $ είναι παραγωγίσιμη και έστω ότι το σύνολο αυτό είναι διάστημα ή ένωση διαστημάτων. Η παράγωγος της $ f'' $ αν υπάρχει, τότε λέγεται \textbf{δεύτερη παράγωγος} της $ f $ και συμβολίζεται $ f'''' $. Γενικότερα ορίζεται η $ \nu- $οστή παράγωγος της $ f $ ως
\[ f^{(\nu)}=\left[f^{(\nu-1)}\right]''\ \ ,\ \ \nu\geq 3 \]
%# End of file Analysh-Or-Deuterh-N-osth_Paragwgos','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_shmeio','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synexhs_Synarthsh_se_shmeio.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synexhs_Synarthsh_se_shmeio
%@ Database source: Mathematics
Μια συνάρτηση $ f $ ονομάζεται συνεχής σε ένα σημείο $ x_0 $ του πεδίου ορισμού της όταν το όριο της στο $ x_0 $ είναι ίσο με την τιμή της στο σημείο αυτό. Δηλαδή \[ \lim_{x\rightarrow x_0}{f(x)}=f(x_0) \]
%# End of file Analysh-Or-Synexhs_Synarthsh_se_shmeio','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Rythmos_Metabolhs','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Rythmos_Metabolhs.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Rythmos_Metabolhs
%@ Database source: Mathematics
Αν δύο μεταβλητά ποσά $ x,y $ συνδέονται με μια σχέση $ y=f(x) $ όπου η $ f $ είναι παραγωγίσιμη στο $ x_0 $, τότε ονομάζουμε ρυθμό μεταβολής του $ y $ ως προς $ x $ στο $ x_0\in D_f $ την παράγωγο $ f''(x_0) $.
%# End of file Analysh-Or-Rythmos_Metabolhs','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Efaptomenh_Graf_Parastashs','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Efaptomenh_Graf_Parastashs.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Efaptomenh_Graf_Parastashs
%@ Database source: Mathematics
Αν υπάρχει το όριο
\[ \lim_{x\to x_0}{\frac{f(x)-f(x_0)}{x-x_0}} \]
και είναι πραγματικός αριθμός $ \lambda $, τότε ορίζουμε ως εφαπτομένη της $ C_f $ στο σημείο $ A(x_0,f(x_0)) $ την ευθεία που διέρχεται από το $ A $ και έχει συντελεστή διεύθυνσης $ \lambda=f''(x_0) $.
%# End of file Analysh-Or-Efaptomenh_Graf_Parastashs','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Prakseis_Synarthsewn','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Prakseis_Synarthsewn.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Prakseis_Synarthsewn
%@ Database source: Mathematics
Δίνονται δύο συναρτήσεις $ f,g $ με πεδία ορισμού $ A,B $ αντίστοιχα. 
\begin{enumerate}
\item Η συνάρτηση $ f+g $ του αθροίσματος των δύο συναρτήσεων ορίζεται ως η συνάρτηση με τύπο $ (f+g)(x)=f(x)+g(x) $ και πεδίο ορισμού $ D_{f+g}=A\cap B $.
\item Η συνάρτηση $ f-g $ της διαφοράς των δύο συναρτήσεων ορίζεται ως η συνάρτηση με τύπο $ (f-g)(x)=f(x)-g(x) $ και πεδίο ορισμού $ D_{f-g}=A\cap B $.
\item Η συνάρτηση $ f\cdot g $ του γινομένου των δύο συναρτήσεων ορίζεται ως η συνάρτηση με τύπο $ (f\cdot g)(x)=f(x)\cdot g(x) $ και πεδίο ορισμού $ D_{f\cdot g}=A\cap B $.
\item Η συνάρτηση $ \frac{f}{g} $ του πηλίκου των δύο συναρτήσεων ορίζεται ως η συνάρτηση με τύπο $ \left(\frac{f}{g}\right)(x)=\dfrac{f(x)}{g(x)} $ και πεδίο ορισμού $ D_{\frac{f}{g}}=\{x\in A\cap B:g(x)\neq 0\} $.
\end{enumerate}
Αν $ A\cap B=\varnothing $ τότε οι παραπάνω συναρτήσεις δεν ορίζονται.
%# End of file Analysh-Or-Prakseis_Synarthsewn','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_an_diasthma','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synexhs_Synarthsh_se_an_diasthma.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synexhs_Synarthsh_se_an_diasthma
%@ Database source: Mathematics
Μια συνάρτηση $ f $ θα λέγεται συνεχής σε ένα ανοιχτό διάστημα $ (a,\beta) $ εάν είναι συνεχής σε κάθε σημείο του διαστήματος.
%# End of file Analysh-Or-Synexhs_Synarthsh_se_an_diasthma','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Paragwgisimh_Synarthsh.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Paragwgisimh_Synarthsh
%@ Database source: Mathematics
Μια συνάρτηση $ f $ με πεδίο ορισμού $ D_f $ λέγεται παραγωγίσιμη, αν είναι παραγωγίσιμη σε κάθε σημείο $ x_0 $ του πεδίου ορισμού της.
%# End of file Analysh-Or-Paragwgisimh_Synarthsh','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synolo_Timwn','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synolo_Timwn.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synolo_Timwn
%@ Database source: Mathematics
Σύνολο τιμών μιας συνάρτησης $ f $ με πεδίο ορισμού $ A $ λέγεται το σύνολο που περιέχει όλες τις τιμές $ f(x) $ της συνάρτησης για κάθε  $ x\in A $. Συμβολίζεται με $ f(A) $ και είναι
\[ f(A)=\{y\in\mathbb{R}:y=f(x)\ \textrm{για κάθε}\ x\in A\} \]
%# End of file Analysh-Or-Synolo_Timwn','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Gnisiws_Fthinousa','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Gnisiws_Fthinousa.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Gnisiws_Fthinousa
%@ Database source: Mathematics
Μια συνάρτηση $ f $  ονομάζεται γνησίως φθίνουσα σε ένα διάστημα $\Delta $ του πεδίου ορισμού της αν για κάθε $ x_1,x_2\in\Delta $ με $ x_1<x_2 $ ισχύει $ f(x_1)>f(x_2) $:
\[ x_1<x_2\Rightarrow f(x_1)>f(x_2)\ \ ,\ \ \textrm{για κάθε }x_1,x_2\in\mathbb{\varDelta} \]
%# End of file Analysh-Or-Gnisiws_Fthinousa','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synthesh_Synarthsewn','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synthesh_Synarthsewn.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synthesh_Synarthsewn
%@ Database source: Mathematics
Η σύνθεση μιας συνάρτησης $ f $ με μια συνάρτηση $ g $ με πεδία ορισμού $ A,B $ αντίστοιχα, ονομάζεται η συνάρτηση $ g\circ f $ με τύπο και πεδίο ορισμού
\[ (g\circ f)(x)=g(f(x))\ \ ,\ \ D_{g\circ f}=\{x\in\mathbb{R}| x\in A\ \textrm{ και }\ f(x)\in B\} \]
\begin{center}
\begin{tikzpicture}[scale=.6]
\draw(0,0) ellipse (1cm and 1.5cm);
\draw(4,0) ellipse (1cm and 1.5cm);
\begin{scope}
\draw[clip](4,0) ellipse (1cm and 1.5cm);
\draw[fill=\xrwma!30] (5,0) ellipse (1cm and 1.5cm);
\end{scope}
\draw (5,0) ellipse (1cm and 1.5cm);
\draw (9,0) ellipse (1cm and 1.5cm);
\draw (-.1,0)[fill=\xrwma!30] ellipse (.6cm and 1.1cm);
\draw[-latex] (0,.2) arc (140:40:2.95);
\draw[-latex] (4.5,.2) arc (140:40:2.95);
\draw[-latex] (0,.2) arc (140:40:5.9);
\tkzDefPoint(0,.2){A}
\tkzDefPoint(4.5,.15){B}
\tkzDefPoint(9,.15){C}
\tkzDrawPoints(A,B,C)
\tkzLabelPoint[left](A){{\footnotesize $ x $}}
\tkzLabelPoint[below](B){{\footnotesize $ f(x) $}}
\tkzLabelPoint[below](C){{\footnotesize $ g(f(x)) $}}
\tkzText(0,1.8){\footnotesize$ A $}
\tkzText(3.8,1.8){\footnotesize$ f(A) $}
\tkzText(5.2,1.8){\footnotesize$ B $}
\tkzText(2.4,1.55){\footnotesize$ f $}
\tkzText(6.4,1.55){\footnotesize$ g $}
\tkzText(4.5,2.55){\footnotesize$ g\circ f $}
\tkzText(9,1.8){\footnotesize$ g(B) $}
\draw[-latex] (4.5,-0.8) -- (3.2,-1.9) node[anchor=east] {\footnotesize $ f\left( A \right)\cap B  $};
\draw[-latex] (0,-.5) -- (-.8,-1.7) node[anchor=east,xshift=1mm] {\footnotesize $ D_{g\circ f}  $};
\end{tikzpicture}
\end{center}
\begin{itemize}[itemsep=0mm]
\item Διαβάζεται «σύνθεση της $ f $ με τη $ g $» ή «$ g $ σύνθεση $ f $».
\item Για να ορίζεται η συνάρτηση $ g\circ f $ θα πρέπει να ισχύει $ f(A)\cap B\neq\varnothing $.
\item Αντίστοιχα ορίζεται και η σύνθεση $ f\circ g $ με πεδίο ορισμού το $ D_{f\circ g}=\{x\in\mathbb{R}|x\in B\ \ \textrm{και}\ \ g(x)\in A\} $ και τύπο $ (f\circ g)(x)=f(g(x)) $.
\end{itemize}
%# End of file Analysh-Or-Synthesh_Synarthsewn','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma
%@ Database source: Mathematics
Μια συνάρτηση $ f $ θα λέγεται συνεχής σε ένα κλειστό διάστημα $ [a,\beta] $ εάν είναι συνεχής σε κάθε σημείο του ανοιχτού διαστήματος $ (a,\beta) $ και επιπλέον ισχύει
\[ \lim_{x\to a^+}{f(x)}=f(a)\ \ \textrm{και}\ \ \lim_{x\to\beta^-}{f(x)}=f(\beta) \]
%# End of file Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_synolo','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synexhs_Synarthsh_se_synolo.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synexhs_Synarthsh_se_synolo
%@ Database source: Mathematics
Μια συνάρτηση $ f $ θα λέμε ότι είναι \textbf{συνεχής} εάν είναι συνεχής σε κάθε σημείο του πεδίου ορισμού της.
%# End of file Analysh-Or-Synexhs_Synarthsh_se_synolo','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Synarthsh_1-1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Synarthsh_1-1.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Synarthsh_1-1
%@ Database source: Mathematics
Μια συνάρτηση $ f:A\rightarrow\mathbb{R} $ ονομάζεται $ 1-1 $ εάν για κάθε ζεύγος αριθμών $ x_1,x_2\in A $ του πεδίου ορισμού της θα ισχύει \[ x_1\neq x_2\Rightarrow f(x_1)\neq f(x_2) \]
Δηλαδή κάθε στοιχείο $ x\in A $ του πεδίου ορισμού αντιστοιχεί μέσω της συνάρτησης, σε μοναδική τιμή $ f(x) $ του συνόλου τιμών της.
%# End of file Analysh-Or-Synarthsh_1-1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Ises_Synarthseis_1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Ises_Synarthseis_1.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Ises_Synarthseis_1
%@ Database source: Mathematics
Δύο συναρτήσεις $ f,g $ που έχουν το ίδιο πεδίο ορισμού $ A $ ονομάζονται ίσες δηλαδή $ f=g $ όταν ισχύει $ f(x)=g(x) $ για κάθε $ x\in A $.
%# End of file Analysh-Or-Ises_Synarthseis_1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Gnisiws_Auxousa','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Gnisiws_Auxousa.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Gnisiws_Auxousa
%@ Database source: Mathematics
Μια συνάρτηση $ f $  ονομάζεται γνησίως αύξουσα σε ένα διάστημα $\Delta $ του πεδίου ορισμού της αν για κάθε $ x_1,x_2\in\Delta $ με ισχύει $ f(x_1)<f(x_2) $:
\[ x_1<x_2\Rightarrow f(x_1)<f(x_2)\ \ ,\ \ \textrm{για κάθε }x_1,x_2\in\mathbb{\varDelta} \]
%# End of file Analysh-Or-Gnisiws_Auxousa','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Pragmatikh_Synarthsh','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Pragmatikh_Synarthsh.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Pragmatikh_Synarthsh
%@ Database source: Mathematics
Πραγματική συνάρτηση με πεδίο ορισμού ένα σύνολο $ A $ είναι μια διαδικασία (αντιστοίχηση) με την οποία \textbf{κάθε} στοιχείο $ x\in A $ αντιστοιχεί σε \textbf{ένα μόνο} πραγματικό αριθμό $ y\in\mathbb{R} $. Το $ y $ λέγεται \textbf{τιμή} της συνάρτησης $ f $ στο $ x $ και συμβολίζεται $ f(x) $.
\begin{center}
\centering
\begin{tikzpicture}[scale=.6]
\draw(0,0) ellipse (1cm and 1.5cm);
\draw(4,0) ellipse (1cm and 1.5cm);
\draw[fill=\xrwma!30] (4.1,0) ellipse (.6cm and 1.1cm);
\draw[-latex] (0,.2) arc (140:40:2.6);
\tkzDefPoint(0,.2){A}
\tkzDefPoint(4,.2){B}
\tkzDrawPoints(A,B)
\tkzLabelPoint[left](A){{\footnotesize $ x $}}
\tkzLabelPoint[right](B){{\footnotesize $ y $}}
\tkzText(0,1.8){$ A $}
\tkzText(4,1.8){$ \mathbb{R} $}
\tkzText(2,1.45){$ f $}
\draw[-latex] (3.5,0) -- (2.7,-1) node[anchor=north east] {\footnotesize $ f\left( A \right)  $};
\end{tikzpicture}
\end{center}
%# End of file Analysh-Or-Pragmatikh_Synarthsh','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Oliko_Megisto','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Oliko_Megisto.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Oliko_Megisto
%@ Database source: Mathematics
Έστω μια συνάρτηση $ f $ με πεδίο ορισμού ένα σύνολο $ A $ και έστω $ x_0\in A $. Η $ f $ θα λέμε ότι παρουσιάζει ολικό μέγιστο στη θέση $ x_0 $, το $ f(x_0) $ όταν 
\[ f(x)\leq f(x_0)\ \ \textrm{για κάθε}\ \ x\in A \]
Το $ x_0 $ λέγεται \textbf{θέση} του μέγιστου.
%# End of file Analysh-Or-Oliko_Megisto','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Olika_Akrotata','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Olika_Akrotata.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Olika_Akrotata
%@ Database source: Mathematics
Ολικά ακρότατα μιας συνάρτησης $f$ ονομάζονται το ολικό μέγιστο και το ολικό ελάχιστο της.
%# End of file Analysh-Or-Olika_Akrotata','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Gnisiws_Monotonh','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Gnisiws_Monotonh.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Gnisiws_Monotonh
%@ Database source: Mathematics
Μια συνάρτηση $f$ λέγεται γνησίως μονότονη σε ένα διάστημα $\varDelta$ εαν είναι γνησίως αύξουσα ή γνησίως φθίνουσα στο $\varDelta$.
%# End of file Analysh-Or-Gnisiws_Monotonh','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Paragwgos_synarthsh','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Paragwgos_synarthsh.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Paragwgos_synarthsh
%@ Database source: Mathematics
Έστω $ A_1 $ το σύνολο όλων των σημείων του $ D_f $ της $ f $ για τα οποία η $ f $ είναι παραγωγίσιμη. Αντιστοιχίζοντας κάθε $ x\in A_1 $ στο $ f''(x) $ ορίζουμε τη συνάρτηση
\[ f'':A_1\to \mathbb{R} \ \ ,\ \ x\to f''(x) \]
η οποία ονομάζεται πρώτη παράγωγος της $ f $.
%# End of file Analysh-Or-Paragwgos_synarthsh','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Paragwgos_se_shmeio','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Paragwgos_se_shmeio.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Paragwgos_se_shmeio
%@ Database source: Mathematics
Μια συνάρτηση $f$ ονομάζεται παραγωγίσιμη σε ένα σημείο $ x_0 $ του πεδίου ορισμού της αν το όριο
\[ \lim_{x\to x_0}{\frac{f(x)-f(x_0)}{x-x_0}} \]
υπάρχει και είναι πραγματικός αριθμός. Το όριο αυτό ονομάζεται \textbf{παράγωγος} της $ f $ στο $ x_0 $ και συμβολίζεται με $ f''(x_0) $.
\[ f''(x_0)=\lim_{x\to x_0}{\frac{f(x)-f(x_0)}{x-x_0}} \]
%# End of file Analysh-Or-Paragwgos_se_shmeio','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Topiko_Elaxisto','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Topiko_Elaxisto.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Topiko_Elaxisto
%@ Database source: Mathematics
Μια συνάρτηση $ f $, με πεδίο ορισμού $ D_f $, θα λέμε ότι παρουσιάζει τοπικό ελάχιστο στο $ x_0\in D_f $, όταν υπάρχει $ \delta>0 $ τέτοιο ώστε
\[ f(x)\geq f(x_0)\ \ \textrm{για κάθε }x\in D_f\cap(x_0-\delta,x_0+\delta) \]
Το $ x_0 $ λέγεται \textbf{θέση} τοπικού ελάχιστου, ενώ το $ f(x_0) $ τοπικό ελάχιστο της $ f $.
%# End of file Analysh-Or-Topiko_Elaxisto','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Or-Topiko_Megisto','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ορισμοί/Analysh-Or-Topiko_Megisto.tex','09/7/2020 17:55','','','%# Database File : Analysh-Or-Topiko_Megisto
%@ Database source: Mathematics
Μια συνάρτηση $ f $, με πεδίο ορισμού $ D_f $, θα λέμε ότι παρουσιάζει τοπικό μέγιστο στο $ x_0\in D_f $, όταν υπάρχει $ \delta>0 $ τέτοιο ώστε
\[ f(x)\leq f(x_0)\ \ \textrm{για κάθε }x\in D_f\cap(x_0-\delta,x_0+\delta) \]
Το $ x_0 $ λέγεται \textbf{θέση} τοπικού μέγιστου, ενώ το $ f(x_0) $ τοπικό μέγιστο της $ f $.
%# End of file Analysh-Or-Topiko_Megisto','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-MonotAkrot-MonAkrGrPar-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Μονοτονία ακρότατα συνάρτησης/Ασκήσεις/Μονοτονία και ακρότατα από γραφική παράσταση/Ana-MonotAkrot-MonAkrGrPar-SectEx1.tex','14/10/2020 15:47','ΟΧΙ','','%# Database File : Ana-MonotAkrot-MonAkrGrPar-SectEx1----
Να βρεθούν τα διαστήματα στα οποία η συνάρτηση του σχήματος ειναι γνησίως αύξουσα ή γνησίως φθίνουσα.\\
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.,itemsep=0mm]
\item
\begin{tikzpicture}[scale=.7,baseline,line cap=round,line join=round,>=triangle 45,x=1.0cm,y=1.0cm]
\draw[-latex,color=black] (-2.5,0) -- (2.5,0);
\foreach \x in {-2,-1,1,2}
\draw[shift={(\x,0)},color=black] (0pt,2pt) -- (0pt,-2pt) node[below] {\footnotesize $\x$};
\draw[-latex,color=black] (0,-1) -- (0,4.5);
\foreach \y in {-1,1,2,3,4}
\draw[shift={(0,\y)},color=black] (2pt,0pt) -- (-2pt,0pt) node[left] {\footnotesize $\y$};
\draw[color=black] (0pt,-10pt) node[right] {\footnotesize $0$};
\clip(-2.5,-1) rectangle (2.5,4.5);
\draw[line width=1.2pt,color=royalblue, smooth,samples=100,domain=-2.0:2.0] plot(\x,{(\x)^2});
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (-2,0)-- (-2,4);
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (2,0)-- (2,4);
\end{tikzpicture}
\item
\begin{tikzpicture}[scale=.7,baseline,line cap=round,line join=round,>=triangle 45,x=1.2cm,y=.8cm]
\draw[-latex,color=black] (-2,0) -- (2,0);
\foreach \x in {-2,-1,1}
\draw[shift={(\x,0)},color=black] (0pt,2pt) -- (0pt,-2pt) node[below] {\footnotesize $\x$};
\draw[-latex,color=black] (0,-3.54) -- (0,3.5);
\foreach \y in {-3,-2,-1,1,2,3}
\draw[shift={(0,\y)},color=black] (2pt,0pt) -- (-2pt,0pt) node[left] {\footnotesize $\y$};
\draw[color=black] (0pt,-10pt) node[right] {\footnotesize $0$};
\clip(-2,-3.54) rectangle (2,3.5);
\draw[line width=1.2pt,color=royalblue, smooth,samples=100,domain=-1.5:1.5] plot(\x,{(\x)^3});
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (-1.5,-3.38)-- (-1.5,0);
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (1.5,3.38)-- (1.5,0);
\end{tikzpicture}
\item
\begin{tikzpicture}[scale=.7,baseline,line cap=round,line join=round,>=triangle 45,x=1.2cm,y=1.0cm]
\draw[-latex,color=black] (-2.5,0) -- (2.5,0);
\foreach \x in {-2,-1,1,2}
\draw[shift={(\x,0)},color=black] (0pt,2pt) -- (0pt,-2pt) node[below] {\footnotesize $\x$};
\draw[-latex,color=black] (0,-2.77) -- (0,4.27);
\foreach \y in {-2,-1,1,2,3,4}
\draw[shift={(0,\y)},color=black] (2pt,0pt) -- (-2pt,0pt) node[left] {\footnotesize $\y$};
\draw[color=black] (0pt,-10pt) node[right] {\footnotesize $0$};
\clip(-2.5,-2.77) rectangle (2.5,4.27);
\draw[line width=1.2pt,color=royalblue, smooth,samples=100,domain=-2.0:2.0] plot(\x,{(\x)^4-3*(\x)^2});
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (-2,0)-- (-2,4);
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (2,0)-- (2,4);
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (-1.22,-2.25)-- (-1.22,0);
\draw [line width=0.4pt,dash pattern=on 2pt off 2pt] (1.22,-2.25)-- (1.22,0);
\draw (-2,1.1) node[anchor=north west] {{\footnotesize $ -\sqrt{\frac{3}{2}} $}};
\draw (0.5,1.1) node[anchor=north west] {{\footnotesize $ \sqrt{\frac{3}{2}} $}};
\end{tikzpicture}
\item \begin{tikzpicture}[scale=.7,baseline,line cap=round,line join=round,>=triangle 45,x=1.0cm,y=1.5cm]
\draw[-latex,color=black] (-0.57,0) -- (6.91,0);
\foreach \x in {,1,2,3,4,5,6}
\draw[shift={(\x,0)},color=black] (0pt,2pt) -- (0pt,-2pt) node[below] {\footnotesize $\x$};
\draw[-latex,color=black] (0,-1.23) -- (0,1.34);
\foreach \y in {-1,1}
\draw[shift={(0,\y)},color=black] (2pt,0pt) -- (-2pt,0pt) node[left] {\footnotesize $\y$};
\draw[color=black] (0pt,-10pt) node[right] {\footnotesize $0$};
\clip(-0.57,-1.23) rectangle (6.91,1.34);
\draw[line width=1.2pt,color=royalblue, smooth,samples=100,domain=0.0:6.283185307179586] plot(\x,{sin(((\x))*180/pi)});
\draw [line width=0.4pt,dash pattern=on 1pt off 1pt] (1.57,1)-- (1.57,0);
\draw [line width=0.4pt,dash pattern=on 1pt off 1pt] (4.71,-1)-- (4.71,0);
\draw (1.2,0.02) node[anchor=north west] {{\footnotesize $\frac{\pi}{2}$}};
\draw (4.2,0.6) node[anchor=north west] {{\footnotesize $\frac{3\pi}{2}$}};
\end{tikzpicture}
\item \begin{tikzpicture}[scale=.7,baseline,line cap=round,line join=round,>=triangle 45,x=1.0cm,y=1.5cm]
\draw[-latex,color=black] (-1,0) -- (5.5,0);
\foreach \x in {-1,1,2,3,4,5}
\draw[shift={(\x,0)},color=black] (0pt,2pt) -- (0pt,-2pt) node[below] {\footnotesize $\x$};
\draw[-latex,color=black] (0,-1) -- (0,3.5);
\foreach \y in {-1,1,2,3}
\draw[shift={(0,\y)},color=black] (2pt,0pt) -- (-2pt,0pt) node[left] {\footnotesize $\y$};
\draw[color=black] (0pt,-10pt) node[right] {\footnotesize $0$};
\clip(-1,-1) rectangle (11,3.5);
\draw[line width=1.2pt,color=royalblue, smooth,samples=100,domain=0.0:5.0] plot(\x,{sqrt((\x))});
\end{tikzpicture}
\end{enumerate}
\end{multicols}
%# End of file Ana-MonotAkrot-MonAkrGrPar-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-MonotAkrot-Monoton-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Μονοτονία ακρότατα συνάρτησης/Ασκήσεις/Μελέτη μονοτονίας συνάρτησης με ορισμό/Ana-MonotAkrot-Monoton-SectEx1.tex','15/10/2020 00:57','ΟΧΙ','','%# Database File : Ana-MonotAkrot-Monoton-SectEx1----
Να μελετηθούν οι παρακάτω συναρτήσεις ως προς τη μονοτονία.
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.,itemsep=0mm]
\item $ f(x)=3x-1 $
\item $ f(x)=2-x $
\item $ f(x)=x^3+3x $
\item $ f(x)=x^3-1 $
\item $ f(x)=-x^3 $
\item $ f(x)=x^2-2\;,\;x>0 $
\item $ f(x)=\sqrt{x}\;,\;x>0 $
\item $ f(x)=\sqrt{3-x}\;,\;x<3 $
\item $ f(x)=\dfrac{2}{x}\;,\;x>0 $
\end{enumerate}
\end{multicols}
%# End of file Ana-MonotAkrot-Monoton-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-MonotAkrot-AkrotAnis-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Μονοτονία ακρότατα συνάρτησης/Ασκήσεις/Εύρεση ακροτάτων με βασικές ανισότητες/Ana-MonotAkrot-AkrotAnis-SectEx1.tex','15/10/2020 01:09','ΟΧΙ','','%# Database File : Ana-MonotAkrot-AkrotAnis-SectEx1----
Να δειχθεί ότι η συνάρτηση
\begin{alist}
\item $ f(x)=x^2-1 $ παίρνει ελάχιστη τιμή για $ x=0 $.
\item $ f(x)=x^2-4x+5 $ παίρνει ελάχιστη τιμή για $ x=2 $.
\item $ f(x)=4x-x^2 $ παίρνει μέγιστη τιμή για $ x=2 $.
\item $ f(x)=x^4-6x^3+10x^2-6x+9 $ παίρνει ελάχιστη τιμή για $ x=3 $.
\item $ f(x)=-x^3+4x^2+3x\;, x>0 $ παίρνει μέγιστη τιμή για $ x=3 $.
\item $ f(x)=|x-2|-1 $ παίρνει ελάχιστη τιμή για $ x=2 $.
\end{alist}
%# End of file Ana-MonotAkrot-AkrotAnis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-PedioOrism-SectEx5','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Πεδίο ορισμού συνάρτησης/Ana-LogarSyn-PedioOrism-SectEx5.tex','21/8/2020 16:23','ΟΧΙ','','%# Database File : Ana-LogarSyn-PedioOrism-SectEx5----
Να βρεθεί το πεδίο ορισμού των παρακάτω λογαριθμικών συναρτήσεων.
\begin{rlist}
\item $ f(x)=\log{|x|} $
\item $ g(x)=\log{|x+3| } $
\item $ h(x)=\log{|2x-1|-3} $
\item $ d(x)=\log{\left| x^2-x-2\right|  } $
\end{rlist}
%# End of file Ana-LogarSyn-PedioOrism-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-PedioOrism-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Πεδίο ορισμού συνάρτησης/Ana-LogarSyn-PedioOrism-SectEx1.tex','21/8/2020 16:23','ΟΧΙ','','%# Database File : Ana-LogarSyn-PedioOrism-SectEx1----
Να βρεθεί το πεδίο ορισμού των παρακάτω λογαριθμικών συναρτήσεων.
\begin{rlist}
\item $ f(x)=\log{(x-2)} $
\item $ g(x)=\log{(3-x)} $
\item $ h(x)=\log{3x-4} $
\item $ d(x)=\log{\left[ 4(x-2)+5\right] } $
\end{rlist}
%# End of file Ana-LogarSyn-PedioOrism-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-PedioOrism-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Πεδίο ορισμού συνάρτησης/Ana-LogarSyn-PedioOrism-SectEx2.tex','21/8/2020 16:23','ΟΧΙ','','%# Database File : Ana-LogarSyn-PedioOrism-SectEx2----
Να βρεθεί το πεδίο ορισμού των παρακάτω λογαριθμικών συναρτήσεων.
\begin{rlist}
\item $ f(x)=\log{x^2} $
\item $ g(x)=\log{\left( x^2-4\right) } $
\item $ h(x)=\log{\left( x^2-x+2\right) } $
\item $ d(x)=\log{\left( x^2+6x+9\right) } $
\item $ r(x)=\log{\left(x^2+3x+5\right) } $
\end{rlist}
%# End of file Ana-LogarSyn-PedioOrism-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-PedioOrism-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Πεδίο ορισμού συνάρτησης/Ana-LogarSyn-PedioOrism-SectEx3.tex','21/8/2020 16:23','ΟΧΙ','','%# Database File : Ana-LogarSyn-PedioOrism-SectEx3----
Να βρεθεί το πεδίο ορισμού των παρακάτω λογαριθμικών συναρτήσεων.
\begin{rlist}
\item $ f(x)=\log{\frac{3}{x-1}} $
\item $ g(x)=\log{\frac{x-1}{x+2}} $
\item $ h(x)=\log{\frac{2x-1}{x^2}} $
\item $ d(x)=\log{\left( 1+\frac{1}{x}\right) } $
\end{rlist}
%# End of file Ana-LogarSyn-PedioOrism-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-PedioOrism-SectEx4','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Πεδίο ορισμού συνάρτησης/Ana-LogarSyn-PedioOrism-SectEx4.tex','21/8/2020 16:23','ΟΧΙ','','%# Database File : Ana-LogarSyn-PedioOrism-SectEx4----
Να βρεθεί το πεδίο ορισμού των παρακάτω λογαριθμικών συναρτήσεων.
\begin{rlist}
\item $ f(x)=\log{\sqrt{x}} $
\item $ g(x)=\log{\left( \sqrt{x}-2\right) } $
\item $ h(x)=\log{\sqrt{x-2}} $
\item $ d(x)=\log{\sqrt{x^2-3x} } $
\end{rlist}
%# End of file Ana-LogarSyn-PedioOrism-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx8','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx8.tex','20/7/2020 17:58','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx8----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0$ με $ x\neq 1 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{x}{8}=3 $
\item $ \log_{x}{4}=2 $
\item $ \log_{x}{27}=3 $
\item $ \log_{x}{64}=3 $
\item $ \log_{x}{625}=4 $
\item $ \log_{x}{343}=3 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx8','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx14','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx14.tex','20/7/2020 18:13','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx14----
Να υπολογίσετε τον πραγματικό αριθμό $ x $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{x-1}{4}=2 $
\item $ \log_{2x-1}{27}=3 $
\item $ \log_{3-x}{16}=2 $
\item $ \log_{x^2}{81}=2 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx14','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx2.tex','20/7/2020 17:51','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx2----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log{x}=3 $
\item $ \log{x}=-2 $
\item $ \ln{x}=4 $
\item $ \ln{x}=-3 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx5','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx5.tex','20/7/2020 17:56','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx5----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{\sqrt{2}}{x}=3 $
\item $ \log_{\sqrt{3}}{x}=2 $
\item $ \log_{\sqrt[3]{2}}{x}=6 $
\item $ \log_{\sqrt[5]{4}}{x}=15 $
\item $ \log_{\sqrt{5}}{x}=-3 $
\item $ \log_{\sqrt{3}}{x}=-4 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx11','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx11.tex','20/7/2020 18:11','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx11----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0$ με $ x\neq 1 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{x}{\frac{4}{25}}=2 $
\item $ \log_{x}{\frac{125}{64}}=3 $
\item $ \log_{x}{\frac{81}{16}}=-4 $
\item $ \log_{x}{1000}=-3 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx11','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx16','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx16.tex','20/7/2020 18:14','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx16----
Να υπολογίσετε τον πραγματικό αριθμό $ x $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \log_{|x-2|}{25}=2 $
\item $ \log_{x^2-3x+4}{2}=2 $
\item $ \log_{x^3-1}{7}=1 $
\item $ \log_{\sqrt[3]{x^2-2x}}{9}=6 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx16','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx15','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx15.tex','20/7/2020 18:14','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx15----
Να υπολογίσετε τον πραγματικό αριθμό $ x $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}[leftmargin=4mm]
\item $ \log_{\sqrt{x}}{5}=2 $
\item $ \log_{\sqrt[3]{2-x}}{7}=3 $
\item $ \log_{\sqrt[4]{3x-4}}{16}=8 $
\item $ \log_{\sqrt{x^2-3}}{13}=2 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx15','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx3.tex','20/7/2020 17:52','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx3----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{0{,}1}{x}=2 $
\item $ \log_{0{,}2}{x}=4 $
\item $ \log_{0{,}5}{x}=3 $
\item $ \log_{0{,}2}{x}=-2 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx6','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx6.tex','20/7/2020 17:56','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx6----
Να υπολογίσετε τον πραγματικό αριθμό $ x $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \log_{2}{(x-1)}=3 $
\item $ \log_{3}{(2x-3)}=2 $
\item $ \log_{4}{(4-x)}=2 $
\item $ \log{\left( x^2-24\right) }=3 $
\item $ \ln{ex}=2 $
\item $ \log{x^3}=9 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx12','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx12.tex','20/7/2020 18:12','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx12----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0$ με $ x\neq 1 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{x}{0{,}04}=2 $
\item $ \log_{x}{0{,}125}=3 $
\item $ \log_{x}{0{,}0001}=4 $
\item $ \log_{x}{1000}=-3 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx12','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx9','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx9.tex','20/7/2020 17:59','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx9----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0$ με $ x\neq 1 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{x}{100}=2 $
\item $ \log_{x}{10^8}=8 $
\item $ \log_{x}{e^3}=3 $
\item $ \log_{x}{e}=1 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx9
','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx1.tex','20/7/2020 17:49','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx1----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{2}{x}=3 $
\item $ \log_{3}{x}=2 $
\item $ \log_{3}{x}=4 $
\item $ \log_{5}{x}=3 $
\item $ \log_{4}{x}=-3 $
\item $ \log_{2}{x}=-5 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx13','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx13.tex','20/7/2020 18:12','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx13----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0 $ με $ x\neq 1 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{x}{2}=2 $
\item $ \log_{x}{3}=3 $
\item $ \log_{x}{10}=2 $
\item $ \log_{x}{e}=2 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx13','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx7','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx7.tex','20/7/2020 17:57','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx7----
Να υπολογίσετε τον πραγματικό αριθμό $ x $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}[leftmargin=1mm]
\item $ \log_{2}{|x-3|}=2 $
\item $ \log_{3}{\sqrt{x-2}}=2 $
\item $ \log_{5}{\left(10x-x^2\right) }=2 $
\item $ \log{\frac{1}{x}}=2 $
\item $ \log_{9}{\sqrt[3]{3x-1}}=\frac{1}{2} $
\item $ \log{\frac{x-2}{x+3}}=1 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx7','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx4','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx4.tex','20/7/2020 17:53','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx4----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{\frac{3}{4}}{x}=2 $
\item $ \log_{\frac{2}{5}}{x}=3 $
\item $ \log_{\frac{1}{3}}{x}=4 $
\item $ \log_{\frac{1}{10}}{x}=-2 $
\item $ \log_{\frac{1}{8}}{x}=-3 $
\item $ \log_{\frac{1}{e}}{x}=-4 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-LogarSyn-LogarExis-SectEx10','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Λογαριθμική συνάρτηση/Ασκήσεις/Λογαριθμικές εξισώσεις/Ana-LogarSyn-LogarExis-SectEx10.tex','20/7/2020 18:10','ΟΧΙ','','%# Database File : Ana-LogarSyn-LogarExis-SectEx10----
Να υπολογίσετε τον πραγματικό αριθμό $ x>0$ με $ x\neq 1 $ στις παρακάτω παραστάσεις.
\begin{multicols}{2}
\begin{rlist}
\item $ \log_{x}{4}=-2 $
\item $ \log_{x}{25}=-2 $
\item $ \log_{x}{\frac{1}{64}}=-3 $
\item $ \log_{x}{\frac{1}{49}}=-2 $
\item $ \log_{x}{\frac{1}{100}}=-2 $
\item $ \log_{x}{e^2}=-2 $
\end{rlist}
\end{multicols}
%# End of file Ana-LogarSyn-LogarExis-SectEx10','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-MonadRiz-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Μοναδική ρίζα/Ana-ThBolzano-MonadRiz-SectEx1.tex','25/11/2020 22:01','ΝΑΙ','','%# Database File : Ana-ThBolzano-MonadRiz-SectEx1----
Να δείξετε ότι η εξίσωση
\[ e^x=2-x \]
έχει μοναδική λύση στο διάστημα $ (0,1) $.
%# End of file Ana-ThBolzano-MonadRiz-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizKlDiast-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ρίζας σε κλειστό διάστημα/Ana-ThBolzano-RizKlDiast-SectEx1.tex','25/11/2020 15:52','ΝΑΙ','','%# Database File : Ana-ThBolzano-RizKlDiast-SectEx1----
Δίνεται η συνάρτηση $ f:\mathbb{R}\to\mathbb{R} $ με $ f(x)=\hm{x} $. Να δείξετε ότι η συνάρτηση $ f $ έχει μια τουλάχιστον ρίζα στο κλειστό διάστημα $ [-a,a] $ με $ a>0 $.
%# End of file Ana-ThBolzano-RizKlDiast-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-BolzLim-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Θ. Bolzano με χρήση ορίων/Ana-ThBolzano-BolzLim-SectEx1.tex','26/11/2020 00:06','ΝΑΙ','','%# Database File : Ana-ThBolzano-BolzLim-SectEx1----
Να αποδείξετε ότι η συνάρτηση $ f:(0,+\infty)\to\mathbb{R} $ με 
\[ f(x)=\ln{x}+x \] έχει μια τουλάχιστον ρίζα στο διάστημα $ (0,1) $.
%# End of file Ana-ThBolzano-BolzLim-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizaExis-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας/Ana-ThBolzano-RizaExis-SectEx2.tex','25/11/2020 21:51','ΝΑΙ','','%# Database File : Ana-ThBolzano-RizaExis-SectEx2----
Να δείξετε ότι υπάρχει τουλάχιστον ένα $ x_0\in(-1,0) $ τέτοιο ώστε να ισχύει
\[ e^{x_0}=\hm{(\pi x_0)}-2x_0 \]
%# End of file Ana-ThBolzano-RizaExis-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizaExis-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας/Ana-ThBolzano-RizaExis-SectEx1.tex','25/11/2020 15:54','ΝΑΙ','','%# Database File : Ana-ThBolzano-RizaExis-SectEx1----
Να αποδείξετε ότι η εξίσωση \[ x^2-\syn{(x\pi)}=e^x \] έχει μια τουλάχιστον λύση στο διάστημα $ (-2,0) $.
%# End of file Ana-ThBolzano-RizaExis-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizaExis-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας/Ana-ThBolzano-RizaExis-SectEx3.tex','25/11/2020 21:55','ΝΑΙ','','%# Database File : Ana-ThBolzano-RizaExis-SectEx3----
Να δειχθεί ότι η εξίσωση
\[ \frac{e^x}{x-1}=x^2-3 \]
έχει μια τουλάχιστον λύση στο διάστημα $ (0,1) $.
%# End of file Ana-ThBolzano-RizaExis-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ρίζας σε ανοιχτό διάστημα/Ana-ThBolzano-RizAnDiast-SectEx1.tex','25/11/2020 15:44','ΝΑΙ','','%# Database File : Ana-ThBolzano-RizAnDiast-SectEx1----
Δίνεται η συνάρτηση $ f $ με $ f(x)=x^3-2x^2-3x+5 $. Να δειχθεί ότι η $ f $ έχει τουλάχιστον μια ρίζα στο διάστημα $ (2,3) $.
%# End of file Ana-ThBolzano-RizAnDiast-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ρίζας σε ανοιχτό διάστημα/Ana-ThBolzano-RizAnDiast-SectEx3.tex','25/11/2020 21:59','ΝΑΙ','','%# Database File : Ana-ThBolzano-RizAnDiast-SectEx3----
Δίνονται οι συναρτήσεις $ f,g:(1,+\infty)\to\mathbb{R} $ με $ f(x)=x^2-3x $ και $ g(x)=\ln{(x-1)} $ αντίστοιχα. Να δειχθεί οι γραφικές παραστάσεις των δύο συναρτήσεων έχουν τουλάχιστον ένα κοινό σημείο με τετμημένη $ x_0\in(2,4) $.
%# End of file Ana-ThBolzano-RizAnDiast-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ρίζας σε ανοιχτό διάστημα/Ana-ThBolzano-RizAnDiast-SectEx2.tex','25/11/2020 15:46','ΝΑΙ','','%# Database File : Ana-ThBolzano-RizAnDiast-SectEx2----
Δίνεται η συνάρτηση $ f:\mathbb{R}\to\mathbb{R} $ με $ f(x)=ax^3+x $ όπου $ a\neq-1 $. Να δειχθεί ότι η $ f $ έχει μια τουλάχιστον ρίζα στο διάστημα $ (-1,1) $.
%# End of file Ana-ThBolzano-RizAnDiast-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-NRizes-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Ασκήσεις/Ύπαρξη ν ριζών/Ana-ThBolzano-NRizes-SectEx1.tex','26/11/2020 00:09','ΝΑΙ','','%# Database File : Ana-ThBolzano-NRizes-SectEx1----
Δίνεται η συνάρτηση $ f:\mathbb{R}\to\mathbb{R} $ με $ f(x)=e^x-\hm{(\pi x)}-3x $. Να δείξετε ότι η συνάρτηση $ f $ έχει δύο τουλάχιστον ρίζες στο διάστημα $ (0,2) $.
%# End of file Ana-ThBolzano-NRizes-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-MonadRiz-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Μοναδική ρίζα/Ana-ThBolzano-MonadRiz-SolSE1.tex','25/11/2020 22:02','ΟΧΙ','','%# Database File : Ana-ThBolzano-MonadRiz-SolSE1----
Η αρχική εξίσωση γράφεται ισοδύναμα στη μορφή
\[ e^x+x-2=0 \]
και έτσι ορίζουμε τη συνάρτηση $ f:\mathbb{R}\to\mathbb{R} $ με $ f(x)=e^x+x-2 $. Για τη συνάρτηση αυτή θα έχουμε ότι
\begin{rlist}
\item είναι συνεχής στο διάστημα $ [0,1] $ και επιπλέον
\item \begin{itemize}
\item $ f(0)=e^0+0-2=-1<0 $
\item $ f(1)=e^1+1-2=e-1>0 $
\end{itemize}
άρα θα ισχύει $ f(0)\cdot f(1)=1-e<0 $.
\end{rlist}
Έτσι η εξίσωση θα έχει τουλάχιστον μια λύση $ x_0\in(0,1) $. Για να αποδείξουμε τη μοναδικότητα αυτής της λύσης εξετάζουμε τη συνάρτηση ως προς τη μονοτονία της. Έχουμε λοιπόν για κάθε $ x_1,x_2\in\mathbb{R} $ με $ x_1<x_2 $ ότι:
\begin{gather*}
x_1<x_2\Rightarrow e^{x_1}<e^{x_2}\Rightarrow\\ e^{x_1}+x_1<e^{x_2}+x_2\Rightarrow\\ e^{x_1}+x_1-2<e^{x_2}+x_2-2\Rightarrow\\ f(x_1)<f(x_2)
\end{gather*}
Επομένως η συνάρτηση $ f $ είναι γνησίως αύξουσα στο $ \mathbb{R} $ άρα η λύση $ x_0\in(0,1) $ είναι μοναδική.
%# End of file Ana-ThBolzano-MonadRiz-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizKlDiast-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ρίζας σε κλειστό διάστημα/Ana-ThBolzano-RizKlDiast-SolSE1.tex','25/11/2020 15:53','ΟΧΙ','','%# Database File : Ana-ThBolzano-RizKlDiast-SolSE1----
Η συνάρτηση $ f $ έχει πεδίο ορισμού το σύνολο $ \mathbb{R} $. Γι αυτήν επίσης θα έχουμε ότι:
\begin{rlist}
\item είναι συνεχής στο διάστημα $ [-a,a] $ και επιπλέον
\item $ f(-a)=\hm{(-a)} $\ \ και\ \ 
$ f(a)=\hm{a} $.\\
Γνωρίζουμε όμως ότι οι αντίθετες γωνίες $ -a $ και $ a $ έχουν αντίθετα ημίτονα άρα θα ισχύει $ \hm{(-a)}=-\hm{a} $ και έτσι παίρνουμε:
\[ f(-a)\cdot f(a)=\hm{(-a)}\cdot\hm{a}=-\hm^2{a}\leq0 \]
\end{rlist}
Εξετάζουμε τώρα τις παρακάτω περιπτώσεις:
\begin{itemize}
\item Αν $ f(-a)\cdot f(a)<0 $ τότε σύμφωνα με το θεώρημα Bolzano θα υπάρχει τουλάχιστον ένας αριθμός $ x_0 $ στο ανοικτό διάστημα $ (-a,a) $ τέτοιος ώστε
\[ f(x_0)=\hm{x_0}=0 \]
\item Αν $ f(-a)\cdot f(a)=0 $ τότε θα ισχύει $ f(-a)=0 $ ή $ f(a)=0 $ άρα το $ a $ θα είναι ρίζα της $ f $.
\end{itemize}
Από τις δύο παραπάνω περιπτώσεις καταλήγουμε στο συμπέρασμα ότι η ρίζα της συνάρτησης θα ανήκει στο κλειστό διάστημα $ [-a,a] $.
%# End of file Ana-ThBolzano-RizKlDiast-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-BolzLim-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Θ. Bolzano με χρήση ορίων/Ana-ThBolzano-BolzLim-SolSE1.tex','26/11/2020 00:07','ΟΧΙ','','%# Database File : Ana-ThBolzano-BolzLim-SolSE1----
Παρατηρούμε ότι η συνάρτηση δεν ορίζεται στο $ 0 $ το οποίο είναι το κάτω άκρο του διαστήματος. Έτσι υπολογίζουμε το όριο της $ f $ στο $ 0 $ και έχουμε ότι:
\[ \lim_{x\to 0}{f(x)}=\lim_{x\to 0}{(\ln{x}+x)}=-\infty<0 \]
Άρα θα υπάρχει ένας πραγματικός αριθμός $ x_1 $ κοντά στο $ 0 $ έτσι ώστε $ f(x_1)<0 $. Στη συνέχεια εφαρμόζουμε το Θ. Bolzano για τη συνάρτηση $ f $ στο διάστημα $ [x_1,1] $ και ισχύει ότι:
\begin{rlist}
\item η $ f $ είναι συνεχής στο $ [x_1,1] $ ενώ
\item \begin{itemize}
\item $ f(x_1)<0 $
\item $ f(1)=\ln1+1=1>0 $
\end{itemize}
άρα παίρνουμε $ f(x_1)\cdot f(1)<0 $.
\end{rlist}
Έτσι, από το θεώρημα Bolzano, θα υπάρχει τουλάχιστον ένα $ x_0\in(x_1,1)\subseteq(0,1) $ τέτοιο ώστε $ f(x_0)=0 $.
%# End of file Ana-ThBolzano-BolzLim-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizaExis-SolSE3','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας/Ana-ThBolzano-RizaExis-SolSE3.tex','25/11/2020 21:56','ΟΧΙ','','%# Database File : Ana-ThBolzano-RizaExis-SolSE3----
\wrapr{-5mm}{7}{5cm}{-5mm}{\parat
\begin{tcolorbox}[title=\Parathrhsh,hbox,lifted shadow={1mm}{-2mm}{3mm}{0.3mm}%
{black!50!white}]
\begin{varwidth}{4cm}
{\small Οι δύο εξισώσεις είναι ισοδύναμες στο $ (0,1) $ γιατί στο διάστημα αυτό δεν ανήκει το $ x=1 $ του περιορισμού.}
\end{varwidth}
\end{tcolorbox}}{
Για την αρχική εξίσωση απαιτούμε να ισχύει $ x-1\neq0\Rightarrow x\neq1 $. Όμως για κάθε $ x\in(0,1) $ η αρχική μετατρέπεται στην ισοδύναμη εξίσωση:
\begin{equation}\label{par:ex}
e^x=(x-1)\left( x^2-3\right)
\end{equation}
Στη συνέχεια, η τελευταία θα γραφτεί:
\[ e^x-(x-1)\left( x^2-3\right)=0 \]
Ορίζουμε έτσι τη συνάρτηση $ f(x)=e^x-(x-1)\left( x^2-3\right) $ με πεδίο ορισμού το $ \mathbb{R} $. Το θεώρημα Bolzano εφαρμόζεται στο διάστημα $ [0,1] $ και έτσι έχουμε ότι}
\begin{rlist}
\item Η $ f $ είναι συνεχής στο διάστημα $ [0,1] $ και επιπλέον
\item \begin{itemize}
\item $ f(0)=e^0-(0-1)\left( 0^2-3\right)=-2<0 $
\item $ f(1)=e^1-(1-1)\left( 1^2-3\right)=e>0 $
\end{itemize}
οπότε παίρνουμε $ f(0)\cdot f(1)=-2e<0 $.
\end{rlist}
Έτσι σύμφωνα με το θεώρημα Bolzano η εξίσωση \eqref{par:ex} και κατά συνέπεια η αρχική εξίσωση θα έχει μια τουλάχιστον λύση $ x_0 $ στο ανοικτό διάστημα $ (0,1) $.
%# End of file Ana-ThBolzano-RizaExis-SolSE3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizaExis-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας/Ana-ThBolzano-RizaExis-SolSE1.tex','25/11/2020 15:55','ΟΧΙ','','%# Database File : Ana-ThBolzano-RizaExis-SolSE1----
Μεταφέροντας όλους τους όρους της εξίσωσης στο πρώτο μέλος, αυτή θα πάρει τη μορφή:
\[ x^2-\syn{(x\pi)}-e^x=0 \]
Ορίζουμε τη συνάρτηση $ f(x)=x^2-\syn{(x\pi)}-e^x $ με πεδίο ορισμού το $ \mathbb{R} $. Γι αυτήν θα έχουμε ότι
\begin{rlist}
\item είναι συνεχής στο κλειστό διάστημα $ [-2,0] $ και
\item \begin{itemize}
\item $ f(-2)=(-2)^2-\syn{(-2\pi)}-e^{-2}=4-1-e^{-2}=3-\frac{1}{e^2}>0 $
\item $ f(0)=0^2-\syn{0}-e^0=-1-1=-2<0 $
\end{itemize}
οπότε παίρνουμε $ f(-2)\cdot f(0)=-2\left(3-\frac{1}{e^2} \right)<0 $.
\end{rlist}
Έτσι σύμφωνα με το θεώρημα του Bolzano η συνάρτηση $ f $ θα έχει μια τουλάχιστον ρίζα $ x_0\in(-2,0) $, ή ισοδύναμα η αρχική εξίσωση θα έχει μια τουλάχιστον λύση $ x_0 $ στο ανοικτό διάστημα $ (-2,0) $.
%# End of file Ana-ThBolzano-RizaExis-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizaExis-SolSE2','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ρίζας εξίσωσης - Επαλήθευση ισότητας/Ana-ThBolzano-RizaExis-SolSE2.tex','25/11/2020 21:52','ΟΧΙ','','%# Database File : Ana-ThBolzano-RizaExis-SolSE2----
Θα σχηματίσουμε από τη ζητούμενη ισότητα την αντίστοιχη εξίσωση θέτοντας όπου $ x_0 $ τη μεταβλητή $ x $. Προκύπτει λοιπόν η εξίσωση
\[ e^{x}=\hm{(\pi x)}-2x\Rightarrow e^{x}-\hm{(\pi x)}+2x=0 \]
Θεωρούμε τη συνάρτηση $ f:\mathbb{R}\to\mathbb{R} $ με τύπο $ f(x)=e^{x}-\hm{(\pi x)}+2x $. Θα ισχύει ότι
\begin{rlist}
\item η $ f $ είναι συνεχής στο διάστημα $ [-1,0] $ και
\item \begin{itemize}
\item $ f(-1)=e^{-1}-\hm{(-\pi)}+2(-1)=\frac{1}{e}-2<0 $
\item $ f(0)=e^0-\hm{0}+2\cdot 0=1>0 $
\end{itemize}
οπότε προκύπτει $ f(-1)\cdot f(0)=\frac{1}{e}-2<0 $
\end{rlist}
Σύμφωνα λοιπόν με το θεώρημα Bolzano η $ f $ θα έχει μια τουλάχιστον ρίζα $ x_0\in(-1,0) $, ή ισοδύναμα η εξίσωση θα έχει μια τουλάχιστον λύση $ x_0 $ στο $ (-1,0) $ άρα τελικά υπάρχει $ x_0\in(-1,0) $ τέτοιο ώστε
\[ e^{x_0}=\hm{(\pi x_0)}-2x_0 \]
%# End of file Ana-ThBolzano-RizaExis-SolSE2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE3','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ρίζας σε ανοιχτό διάστημα/Ana-ThBolzano-RizAnDiast-SolSE3.tex','25/11/2020 21:59','ΟΧΙ','','%# Database File : Ana-ThBolzano-RizAnDiast-SolSE3----
Για να υπάρχει τουλάχιστον ένα κοινό σημείο $ A(x_0,f(x_0)) $ των δύο γραφικών παραστάσεων αρκεί ισοδύναμα να υπάρχει τουλάχιστον ένα $ x_0\in(2,4) $ τέτοιο ώστε $ f(x_0)=g(x_0) $. Απαιτούμε λοιπόν να ισχύει $ f(x)=g(x) $ και ορίζουμε τη συνάρτηση
\[ h(x)=f(x)-g(x)=x^2-3x-\ln{(x-1)}\ ,\ x\in(1,+\infty) \]
Για τη συνάρτηση $ h $ έχουμε ότι:
\begin{rlist}
\item είναι συνεχής στο διάστημα $ [2,4] $ και επίσης
\item \begin{itemize}
\item $ h(2)=2^2-3\cdot2-\ln1=-2<0 $
\item $ h(4)=4^2-3\cdot4-\ln{3}=4-\ln{3}>0 $
\end{itemize}
οπότε προκύπτει ότι $ h(2)\cdot h(4)=-2(4-\ln3)<0 $.
\end{rlist}
Έτσι σύμφωνα με το θεώρημα Bolzano υπάρχει τουλάχιστον ένα $ x_0\in(2,4) $ τέτοιο ώστε $ h(x_0)=0 $ ή ισοδύναμα $ f(x_0)=g(x_0) $.
%# End of file Ana-ThBolzano-RizAnDiast-SolSE3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE2','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ρίζας σε ανοιχτό διάστημα/Ana-ThBolzano-RizAnDiast-SolSE2.tex','25/11/2020 15:47','ΟΧΙ','','%# Database File : Ana-ThBolzano-RizAnDiast-SolSE2----
\wrapr{-5mm}{7}{5cm}{-5mm}{\parat
\begin{tcolorbox}[title=\Parathrhsh,hbox,    %%<<---- here
lifted shadow={1mm}{-2mm}{3mm}{0.3mm}%
{black!50!white}]
\begin{varwidth}{4cm}
{\small Παρόλο που δε γνωρίζουμε τις τιμές $ f(-1),f(1) $, το γινόμενό τους είναι μια γνήσια αρνητική παράσταση.}
\end{varwidth}
\end{tcolorbox}}{
Εξετάζουμε όπως προηγουμένως αν πληρούνται οι υποθέσεις του θεωρήματος Bolzano. Η συνάρτηση $ f $ είναι:
\begin{rlist}
\item συνεχής στο διάστημα $ [-1,1] $ και επίσης
\item \begin{itemize}
\item $ f(-1)=a(-1)^3-1=-a-1 $
\item $ f(1)=a\cdot1^3+1=a+1  $
\end{itemize}
οπότε θα ισχύει $ f(-1)\cdot f(1)=(-a-1)(a+1)=-(a+1)^2<0 $ αφού σύμφωνα με την υπόθεση $ a\neq-1 $.
\end{rlist}
Έτσι θα υπάρχει τουλάχιστον ένα $ x_0\in(-1,1) $ τέτοιο ώστε να ισχύει $ f(x_0)=0 $.}
%# End of file Ana-ThBolzano-RizAnDiast-SolSE2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ρίζας σε ανοιχτό διάστημα/Ana-ThBolzano-RizAnDiast-SolSE1.tex','25/11/2020 15:45','ΟΧΙ','','%# Database File : Ana-ThBolzano-RizAnDiast-SolSE1----
Το πεδίο ορισμού της συνάρτησης είναι το $ \mathbb{R} $. Η $ f $ είναι μια συνεχής συνάρτηση σε όλο το $ \mathbb{R} $, ως πολυωνυμική, επομένως
\begin{rlist}
\item είναι συνεχής στο διάστημα $ [2,3] $ και επίσης
\item $ f(2)=2^3-2\cdot 2^2-3\cdot2+5=-1<0 $ \\
$ f(3)=3^3-2\cdot3^2-3\cdot3+5=5>0  $\\
άρα παίρνουμε $ f(2)\cdot f(3)=(-1)\cdot 5=-5<0 $
\end{rlist}
οπότε σύμφωνα με το θεώρημα του Bolzano θα υπάρχει τουλάχιστον ένα $ x_0\in(2,3) $ τέτοιο ώστε να ισχύει
\[ f(x_0)=0 \]
άρα η συνάρτηση $ f $ έχει μια τουλάχιστον ρίζα στο διάστημα $ (2,3) $.
%# End of file Ana-ThBolzano-RizAnDiast-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-ThBolzano-NRizes-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Θεώρημα Bolzano/Λύσεις ασκήσεων ενότητας/Ύπαρξη ν ριζών/Ana-ThBolzano-NRizes-SolSE1.tex','26/11/2020 00:10','ΟΧΙ','','%# Database File : Ana-ThBolzano-NRizes-SolSE1----
\wrapr{-5mm}{7}{5cm}{-8mm}{\tcbset{
enhanced,colback=red!5!white,boxrule=0.1pt,
colframe=\xrwma,fonttitle=\bfseries}
\begin{tcolorbox}[title=\Parathrhsh,hbox,lifted shadow={1mm}{-2mm}{3mm}{0.3mm}%
{black!50!white}]
\begin{varwidth}{4cm}
{\small Οι τιμές $ f(0) $ και $ f(2) $ στα άκρα του αρχικού διαστήματος είναι ομόσημες. Έτσι η επιλογή του ενδιάμεσου σημείου είναι τέτοια ώστε η τιμή του να είναι ετερόσημη με τις προηγούμενες.}
\end{varwidth}
\end{tcolorbox}}{Ως ενδιάμεσο σημείο επιλέγουμε το $ x=1 $ έτσι ώστε να χωρίσουμε το αρχικό διάστημα σε δύο υποδιαστήματα $ [0,1],[1,2] $. Για τη συνάρτηση $ f $ έχουμε ότι:
\begin{rlist}
\item είναι συνεχής στα διαστήματα $ [0,1] $ και $ [1,2] $ ενώ
\item \begin{itemize}
\item $ f(0)=e^0-\hm{0}-3\cdot0=1>0 $
\item $ f(1)=e^1-\hm{\pi}-3\cdot1=e-3<0 $
\item $ f(2)=e^2-\hm{2\pi}-3\cdot2=e^2-6>0 $
\end{itemize}
οπότε προκύπτει ότι $ f(0)\cdot f(1)=e-3<0 $ και $ f(1)\cdot f(2)=(e-3)\left( e^2-6\right)<0 $
\end{rlist}
Σύμφωνα λοιπόν με το θεώρημα του Bolzano υπάρχει τουλάχιστον ένα $ x_1\in(0,1) $ και ένα $ x_2\in(1,2) $ έτσι ώστε $ f(x_1)=f(x_2)=0 $ άρα η $ f $ έχει τουλάχιστον δύο ρίζες στο $ (0,2) $.}
%# End of file Ana-ThBolzano-NRizes-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-OrioShmeio-Or0p0Riz-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Όριο σε σημείο/Ασκήσεις/Απροσδιοριστία 0 προς 0 άρρητης/Ana-OrioShmeio-Or0p0Riz-SectEx1.tex','13/10/2020 18:39','ΝΑΙ','','%# Database File : Ana-OrioShmeio-Or0p0Riz-SectEx1----
Να υπολογίσετε τα όρια των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ \lim\limits_{x\to 1}\frac{x-1}{\sqrt{x}-1} $
\item $ \lim\limits_{x\to 2}\frac{\sqrt{3x-2}-2}{x-2} $
\item $ \lim\limits_{x\to-1}\frac{\sqrt{x+5}-2}{x^2+x} $
\item $ \lim\limits_{x\to 3}{\frac{\sqrt{12-x}-3}{x^2-9}} $
\end{alist}
\end{multicols}
%# End of file Ana-OrioShmeio-Or0p0Riz-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Όριο σε σημείο/Ασκήσεις/Απροσδιοριστία 0 προς 0 ρητής/Ana-OrioShmeio-Orio0p0Pol-SectEx3.tex','13/10/2020 18:27','ΟΧΙ','','%# Database File : Ana-OrioShmeio-Orio0p0Pol-SectEx3----
Να υπολογίσετε τα όρια των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ \lim\limits_{x\to 0}\frac{x^2-x}{2x^3+3x^2} $
\item $ \lim\limits_{x\to -3}\frac{x^3+x^2+4x-6}{x^2-2x-15} $
\item $ \lim\limits_{x\to2}\frac{x^2-4x+4}{2x-x^2} $
\item $ \lim\limits_{x\to \sqrt{2}}{\frac{x^2-2}{x^2-\sqrt{2}x}} $
\end{alist}
\end{multicols}
%# End of file Ana-OrioShmeio-Orio0p0Pol-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Όριο σε σημείο/Ασκήσεις/Απροσδιοριστία 0 προς 0 ρητής/Ana-OrioShmeio-Orio0p0Pol-SectEx1.tex','13/10/2020 18:19','ΟΧΙ','','%# Database File : Ana-OrioShmeio-Orio0p0Pol-SectEx1----
Να υπολογίσετε τα όρια των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ \lim\limits_{x\to 3}\frac{x^2-9}{x^2-3x} $
\item $ \lim\limits_{x\to -1}\frac{x^2+x}{x^2-x-2} $
\item $ \lim\limits_{x\to2}\frac{x^3-8}{x^2-3x+2} $
\item $ \lim\limits_{x\to -4}{\frac{16-x^2}{x^3-4x}} $
\end{alist}
\end{multicols}
%# End of file Ana-OrioShmeio-Orio0p0Pol-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx4','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Όριο σε σημείο/Ασκήσεις/Απροσδιοριστία 0 προς 0 ρητής/Ana-OrioShmeio-Orio0p0Pol-SectEx4.tex','13/10/2020 18:34','ΟΧΙ','','%# Database File : Ana-OrioShmeio-Orio0p0Pol-SectEx4----
Να υπολογίσετε τα όρια των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ \lim\limits_{x\to 1}\frac{x^2-1}{x-1} $
\item $ \lim\limits_{x\to 2}\frac{2-x}{x^2-2x} $
\item $ \lim\limits_{x\to-5}\frac{25-x^2}{x^3+5x^2} $
\item $ \lim\limits_{x\to -2}{\frac{x^3+8}{x^2-4}} $
\end{alist}
\end{multicols}
%# End of file Ana-OrioShmeio-Orio0p0Pol-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Όριο σε σημείο/Ασκήσεις/Απροσδιοριστία 0 προς 0 ρητής/Ana-OrioShmeio-Orio0p0Pol-SectEx2.tex','13/10/2020 18:23','ΟΧΙ','','%# Database File : Ana-OrioShmeio-Orio0p0Pol-SectEx2----
Να υπολογίσετε τα όρια των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ \lim\limits_{x\to -2}\frac{x^2-2x-8}{x^2+2x} $
\item $ \lim\limits_{x\to 1}\frac{x^3-3x^2+5x-2}{1-x} $
\item $ \lim\limits_{x\to2}\frac{x^3-4x}{x^2-4x-12} $
\item $ \lim\limits_{x\to3}{\frac{27-x^2}{x^3-9x}} $
\end{alist}
\end{multicols}
%# End of file Ana-OrioShmeio-Orio0p0Pol-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-OrioShmeio-Or0p0Riz-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Όριο σε σημείο/Λύσεις ασκήσεων ενότητας/Απροσδιοριστία 0 προς 0 άρρητης/Ana-OrioShmeio-Or0p0Riz-SolSE1.tex','14/10/2020 14:07','ΟΧΙ','','%# Database File : Ana-OrioShmeio-Or0p0Riz-SolSE1----
\begin{alist}
\item \[\lim_{x\to 1}\frac{x-1}{\sqrt{x}-1}=\lim_{x\to 1}\frac{(x-1)\left(\sqrt{x}+1\right)}{\left(\sqrt{x}-1\right)\left(\sqrt{x}+1\right)}=\lim_{x\to 1}\frac{(x-1)\left(\sqrt{x}+1\right)}{x-1}=\lim_{x\to 1}\left(\sqrt{x}+1\right)=2 \]
\item \[ \lim_{x\to 2}\frac{\sqrt{3x-2}-2}{x-2}=\lim_{x\to 2}{\frac{\left(\sqrt{3x-2}-2\right)\left(\sqrt{3x-2}+2\right)}{(x-2)\left(\sqrt{3x-2}+2\right)}}=\lim_{x\to 2}\frac{3x-2-4}{(x-2)\left(\sqrt{3x-2}+2\right)} \]
\end{alist}
%# End of file Ana-OrioShmeio-Or0p0Riz-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParPollTyp-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος συνάρτησης πολλαπλού τύπου - απόλυτες τιμές/Ana-Paragogos-ParPollTyp-SectEx2.tex','24/11/2020 15:41','ΟΧΙ','','%# Database File : Ana-Paragogos-ParPollTyp-SectEx2----
Να βρεθεί η πρώτη παράγωγος των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\begin{cdcases}
x^2+x+1 & ,x\geq 0\\ \hm{x}+x & ,x<0
\end{cdcases} $
\item $ f(x)=\begin{cdcases}
x^2-3x & ,x<3\\ \sqrt{x+1}-2 & ,x\geq 3
\end{cdcases} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParPollTyp-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParPollTyp-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος συνάρτησης πολλαπλού τύπου - απόλυτες τιμές/Ana-Paragogos-ParPollTyp-SectEx1.tex','10/11/2020 15:59','ΟΧΙ','','%# Database File : Ana-Paragogos-ParPollTyp-SectEx1----
Να βρεθεί η πρώτη παράγωγος των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=2x-1-|x-3| $
\item $ f(x)=|4-x|-3+4x $
\item $ f(x)=x^2+|2x-1| $
\item $ f(x)=x^2+x-|x-2| $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParPollTyp-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagPilik-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος πηλίκου/Ana-Paragogos-ParagPilik-SectEx1.tex','10/11/2020 13:05','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagPilik-SectEx1----
Για καθεμία από τις παρακάτω συναρτήσεις, να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\dfrac{x}{x-2} $
\item $ f(x)=\dfrac{x^2}{x+1} $
\item $ f(x)=\dfrac{\hm{x}}{e^x} $
\item $ f(x)=\dfrac{\ln{x}}{x} $
\item $ f(x)=\dfrac{x^2+2x}{x^3+4} $
\item $ f(x)=\dfrac{\ef{x}}{\sqrt{x}} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagPilik-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagPilik-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος πηλίκου/Ana-Paragogos-ParagPilik-SectEx3.tex','10/11/2020 15:55','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagPilik-SectEx3----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\dfrac{x\cdot e^x}{x+1} $
\item $ f(x)=\dfrac{e^x\cdot\syn{x}}{x-2} $
\item $ f(x)=\dfrac{x}{e^x\cdot\hm{x}} $
\item $ f(x)=\dfrac{2^x}{x\cdot\syn{x}} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagPilik-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagPilik-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος πηλίκου/Ana-Paragogos-ParagPilik-SectEx2.tex','10/11/2020 14:17','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagPilik-SectEx2----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\dfrac{x^2-3x}{4-x} $
\item $ f(x)=\dfrac{\hm{x}}{\ln{x}} $
\item $ f(x)=\dfrac{\ln{x}+1}{x} $
\item $ f(x)=\dfrac{x^2}{e^x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagPilik-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParSynthRiz-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης ν-οστής ρίζας/Ana-Paragogos-ParSynthRiz-SectEx1.tex','25/11/2020 15:25','ΟΧΙ','','%# Database File : Ana-Paragogos-ParSynthRiz-SectEx1----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\sqrt[3]{x} $
\item $ f(x)=\sqrt[3]{x^2} $
\item $ f(x)=\sqrt[4]{(x-1)^3} $
\item $ f(x)=\sqrt[5]{(2-x)^4} $
\item $ f(x)=\sqrt[3]{(e^x-1)^4} $
\item $ f(x)=\sqrt[5]{(x-3)^4} $
\item $ f(x)=\sqrt[7]{(x^2-3x)^4} $
\item $ f(x)=\sqrt[4]{(\ln{x}-1)^3} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParSynthRiz-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagSynth-SectEx7','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης συνάρτησης (Πίνακας)/Ana-Paragogos-ParagSynth-SectEx7.tex','25/11/2020 01:00','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagSynth-SectEx7----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\ln{x^4} $
\item $ f(x)=\ln{(x^2-3x)} $
\item $ f(x)=\ln{(\hm{x})}\ ,\ x\in(0,\pi) $
\item $ f(x)=\ln{\sqrt{x}} $
\item $ f(x)=\ln{(2^x+x^2)} $
\item $ f(x)=\ln{(\ln{x})} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagSynth-SectEx7','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagSynth-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης συνάρτησης (Πίνακας)/Ana-Paragogos-ParagSynth-SectEx1.tex','25/11/2020 00:15','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagSynth-SectEx1----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=(x^2+4x)^3 $
\item $ f(x)=(x-\hm{x})^5 $
\item $ f(x)=\syn^4{x} $
\item $ f(x)=\ef^3{x} $
\item $ f(x)=(\ln{x}-x)^2 $
\item $ f(x)=\ln^5{x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagSynth-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagSynth-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης συνάρτησης (Πίνακας)/Ana-Paragogos-ParagSynth-SectEx3.tex','25/11/2020 00:20','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagSynth-SectEx3----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\hm{(x+2)} $
\item $ f(x)=\syn{(\ln{x})} $
\item $ f(x)=\hm{(\sqrt{x})} $
\item $ f(x)=\syn{(x^3-4x)} $
\item $ f(x)=\hm{\frac{1}{x}} $
\item $ f(x)=\syn{e^x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagSynth-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagSynth-SectEx6','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης συνάρτησης (Πίνακας)/Ana-Paragogos-ParagSynth-SectEx6.tex','25/11/2020 00:57','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagSynth-SectEx6----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=e^{x^2+2x} $
\item $ f(x)=3^{\sqrt{x}} $
\item $ f(x)=2^{\ln{x}} $
\item $ f(x)=4^{\hm{x}} $
\item $ f(x)=e^{\frac{1}{x}} $
\item $ f(x)=2^{x+\sqrt{x}} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagSynth-SectEx6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagSynth-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης συνάρτησης (Πίνακας)/Ana-Paragogos-ParagSynth-SectEx2.tex','24/11/2020 15:36','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagSynth-SectEx2----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\sqrt{x^2+2x} $
\item $ f(x)=\sqrt{3-x} $
\item $ f(x)=\sqrt{\hm{x}}\ ,\ x\in(0,\pi) $
\item $ f(x)=\sqrt{\ln{x}-1} $
\item $ f(x)=\sqrt{e^x-1} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagSynth-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagSynth-SectEx4','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης συνάρτησης (Πίνακας)/Ana-Paragogos-ParagSynth-SectEx4.tex','25/11/2020 01:02','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagSynth-SectEx4----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\dfrac{1}{x^2} $
\item $ f(x)=\dfrac{2}{x^3-2x} $
\item $ f(x)=\dfrac{1}{\hm{x}} $
\item $ f(x)=\dfrac{3}{\sqrt{x}} $
\item $ f(x)=\dfrac{4}{e^x} $
\item $ f(x)=\dfrac{\pi}{\ln{x}} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagSynth-SectEx4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagSynth-SectEx5','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης συνάρτησης (Πίνακας)/Ana-Paragogos-ParagSynth-SectEx5.tex','25/11/2020 01:02','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagSynth-SectEx5----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\ef{x^3} $
\item $ f(x)=\syf{(\sqrt{x})} $
\item $ f(x)=\ef{e^x} $
\item $ f(x)=\syf{\frac{1}{x}} $
\item $ f(x)=\ef{(\ln{x})} $
\item $ f(x)=\syf{2^x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagSynth-SectEx5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagGin-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος γινομένου/Ana-Paragogos-ParagGin-SectEx1.tex','10/11/2020 15:47','ΝΑΙ','','%# Database File : Ana-Paragogos-ParagGin-SectEx1----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=x^2\cdot e^x $
\item $ f(x)=x\ln{x} $
\item $ f(x)=e^x\cdot\hm{x} $
\item $ f(x)=\sqrt{x}\cdot\syn{x} $
\item $ f(x)=x^3\cdot 2^x $
\item $ f(x)=x\cdot\ef{x} $
\item $ f(x)=\left(x^2-3x\right)\cdot e^x $
\item $ f(x)=\left(x^3-x^2\right)\cdot\hm{x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagGin-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagGin-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος γινομένου/Ana-Paragogos-ParagGin-SectEx2.tex','10/11/2020 15:56','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagGin-SectEx2----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\hm{x}\cdot\syn{x} $
\item $ f(x)=\sqrt{x}\cdot\ln{x} $
\item $ f(x)=e^x\left(x-2\right) $
\item $ f(x)=\syf{x}\cdot\hm{x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParagGin-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParSynEkth-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης εκθετικής/Ana-Paragogos-ParSynEkth-SectEx1.tex','25/11/2020 01:21','ΟΧΙ','','%# Database File : Ana-Paragogos-ParSynEkth-SectEx1----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=x^x $
\item $ f(x)=(x+3)^x $
\item $ f(x)=\left( \sqrt{x}\right) ^{x-2} $
\item $ f(x)=\hm^x{x}\ ,\ x\in(0,\pi) $
\item $ f(x)=x^{\ln{x}} $
\item $ f(x)=(x^2-3x+2)^{x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParSynEkth-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParSynEkth-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος σύνθετης εκθετικής/Ana-Paragogos-ParSynEkth-SectEx2.tex','25/11/2020 15:20','ΟΧΙ','','%# Database File : Ana-Paragogos-ParSynEkth-SectEx2----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η πρώτη παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=(\ln{x}-1)^x $
\item $ f(x)=\left(\sqrt{x-1}-2\right)^x $
\item $ f(x)=x^{\sqrt{x}} $
\item $ f(x)=\left(\hm{x}\right)^{\hm{x}} $
\item $ f(x)=\left(\sqrt{x}\right)^{\sqrt{x}} $
\item $ f(x)=\left(\dfrac{1}{x}\right)^{x-1} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParSynEkth-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParAnTax-SectEx1','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγοι ανώτερης τάξης/Ana-Paragogos-ParAnTax-SectEx1.tex','25/11/2020 15:28','ΟΧΙ','','%# Database File : Ana-Paragogos-ParAnTax-SectEx1----
Να βρεθεί η 2η παράγωγος των παρακάτω συναρτήσεων.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=x^3+4x^2-3x+2 $
\item $ f(x)=\ln{x}+\hm{x} $
\item $ f(x)=x^4-\sqrt{x} $
\item $ f(x)=2x^3+\syn{x}-e^x $
\item $ f(x)=\ef{x} $
\item $ f(x)=\syn{x}-x^2 $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParAnTax-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParAnTax-SectEx2','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγοι ανώτερης τάξης/Ana-Paragogos-ParAnTax-SectEx2.tex','25/11/2020 15:39','ΟΧΙ','','%# Database File : Ana-Paragogos-ParAnTax-SectEx2----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η 2η παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=x^2\cdot e^x $
\item $ f(x)=x\cdot\ln{x} $
\item $ f(x)=e^x\cdot\hm{x} $
\item $ f(x)=\sqrt{x}\cdot\syn{x} $
\item $ f(x)=e^x\cdot\ln{x} $
\item $ f(x)=\hm{x}\cdot\syn{x} $
\item $ f(x)=x^3\cdot\ln{x} $
\item $ f(x)=\sqrt{x}\cdot\ln{x} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParAnTax-SectEx2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParAnTax-SectEx3','SectEx','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγοι ανώτερης τάξης/Ana-Paragogos-ParAnTax-SectEx3.tex','25/11/2020 21:54','ΟΧΙ','','%# Database File : Ana-Paragogos-ParAnTax-SectEx3----
Για καθεμία από τις παρακάτω συναρτήσεις να βρεθεί η 2η παράγωγος.
\begin{multicols}{2}
\begin{alist}
\item $ f(x)=\dfrac{x}{e^x} $
\item $ f(x)=\dfrac{e^x}{x^2} $
\item $ f(x)=\dfrac{x}{x-1} $
\item $ f(x)=\dfrac{\ln{x}}{x} $
\item $ f(x)=\dfrac{\hm{x}}{x} $
\item $ f(x)=\dfrac{x-2}{\sqrt{x}} $
\end{alist}
\end{multicols}
%# End of file Ana-Paragogos-ParAnTax-SectEx3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Paragogos-ParagGin-SolSE1','SolSE','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Παράγωγος συνάρτηση/Λύσεις ασκήσεων ενότητας/Παράγωγος γινομένου/Ana-Paragogos-ParagGin-SolSE1.tex','10/11/2020 00:38','ΟΧΙ','','%# Database File : Ana-Paragogos-ParagGin-SolSE1----
\begin{alist}
\item Η συνάρτηση $ f $ έχει πεδίο ορισμού το $ \mathbb{R} $. Η παράγωγος της θα είναι
\[ f''(x)=\left(x^2\cdot e^x\right)''=\left(x^2\right)''e^x+x^2(e^x)''=2x\cdot e^x+x^2\cdot e^x=\left(x^2+2x\right)e^x\ ,\ D_{f''}=\mathbb{R} \]
\item Για να ορίζεται η συνάρτηση $ f $ πρέπει $ x>0 $ άρα $ D_f = (0,+\infty) $. Στη συνέχεια θα είναι
\[ f''(x)=\left(x\cdot \ln{x}\right)''=(x)''\ln{x}+x\left(\ln{x}\right)''=\ln{x}+x\cdot\frac{1}{x}=\ln{x}+1\ ,\ D_{f''}=(0,+\infty) \]
\item Η συνάρτηση $ f $ έχει πεδίο ορισμού το $ \mathbb{R} $. Έτσι θα είναι
\[ f''(x)=\left(e^x\cdot\hm{x}\right)''=(e^x)''\hm{x}+e^x(\hm{x})''=e^x\cdot\hm{x}+e^x\cdot\syn{x}=e^x\left(\hm{x}+\syn{x}\right)\ ,\ D_{f''}=\mathbb{R} \]
\item Για να ορίζεται η συνάρτηση $ f $ πρέπει $ x\geq 0 $ άρα $ D_f=[0,+\infty) $. Έχουμε λοιπόν
\[ f''(x)=\left(\sqrt{x}\cdot\syn{x}\right)''=\left(\sqrt{x}\right)''\syn{x}+\sqrt{x}(\syn{x})''=\frac{1}{2\sqrt{x}}\cdot\syn{x}+\sqrt{x}(-\hm{x})=\frac{\syn{x}}{2\sqrt{x}}-\sqrt{x}\cdot\hm{x}\ \ ,\ \ D_{f''}=(0,+\infty) \]
\item Η συνάρτηση $ f $ έχει πεδίο ορισμού $ D_f=\mathbb{R} $.
\[ f''(x)=\left(x^3\cdot2^x\right)''=(x^3)''\cdot2^x+x^2\cdot(2^x)''=3x^2\cdot 2^x+x^2\cdot2^x\ln{2}\ \ ,\ \ D_{f''}=\mathbb{R} \]
\end{alist}
%# End of file Ana-Paragogos-ParagGin-SolSE1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Th-Grafikes_Par_Cf_Cf-1','Theor','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Θεωρήματα/Analysh-Th-Grafikes_Par_Cf_Cf-1.tex','09/7/2020 17:55','','','%# Database File : Analysh-Th-Grafikes_Par_Cf_Cf-1----
%@ Database source: Mathematics
\bmath{Να αποδείξετε ότι οι γραφικές παραστάσεις $ C_f $ και $ C_{f^{-1}} $ των συναρτήσεων $ f $ και $ f^{-1} $ είναι συμμετρικές ως προς την ευθεία $ y=x $ που διχοτομεί τις γωνίες $ x\hat{O}y $ και $ x''\hat{O}y'' $.}\\
\textbf{ΑΠΟΔΕΙΞΗ}\\
Έστω μια συνάρτηση $ f $ η οποία είναι $ 1-1 $ άρα και αντιστρέψιμη. Θα ισχύει γι αυτήν ότι
\[ f(x)=y\Rightarrow x=f^{-1}(y) \]
Αν θεωρήσουμε ένα σημείο $ M(a,\beta) $ που ανήκει στη γραφική παράσταση της $ f $ τότε
\[ f(a)=\beta\Rightarrow a=f^{-1}(\beta) \]
κάτι που σημαίνει ότι το σημείο $ M''(\beta,a) $ ανήκει στη γραφική παράσταση της $ f^{-1} $. Τα σημεία όμως $ M $ και $ M'' $ είναι συμμετρικά ως προς της ευθεία $ y=x $ που διχοτομεί τις γωνίες $ x\hat{O}y $ και $ x;\hat{O}y'' $. Άρα οι $ C_f $ και $ C_{f^{-1}} $ είναι συμμετρικές ως προς την ευθεία αυτή.
%# End of file Analysh-Th-Grafikes_Par_Cf_Cf-1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-Method-Select file name','Method','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Sections/Εξισώσεις 1ου βαθμού/Μέθοδοι/Alg-Exis1ou-Method-Select file name.tex','23/1/21 1:34 Π.Μ.','','','%# Database File : Alg-Exis1ou-Method-Select file name----
%#--------------------------------------------------
%#--------------------------------------------------%# Στοιχεία αρχείου Alg-Exis1ou-Method-Select file name--------
%#--------------------------------------------------%# End of file Alg-Exis1ou-Method-Select file name','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-ExAn-Exis1ou-EpilExis-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Εξισώσεις 1ου βαθμού/Ασκήσεις/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-ExAn-Exis1ou-EpilExis-SectEx2.tex','23/1/2021 01:35','ΟΧΙ','',NULL,'Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1','CombEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συνδυαστικές ασκήσεις/Ανισώσεις 1ου βαθμού-Ανισώσεις 2ου βαθμού/Algebra-Anis1ouAnis2ou-CombEx1.tex','26/1/2021 00:28','NAI','','%# Database File : Algebra-Anis1ouAnis2ou-CombEx1----
Να βρεθεί.
%# End of file Algebra-Anis1ouAnis2ou-CombEx1','Basic','PdfLaTeX',NULL,1,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1','SolCE','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Λυσεις συνδυαστικών ασκήσεων/Ανισώσεις 1ου βαθμού-Ανισώσεις 2ου βαθμού/Algebra-Anis1ouAnis2ou-SolCE1.tex','02/2/2021 00:08','','','%# Database File : Algebra-Anis1ouAnis2ou-SolCE1----
f
%# End of file Algebra-Anis1ouAnis2ou-SolCE1','Basic','PdfLaTeX',NULL,1,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Pithanothtes-Table-Endexomena','Tab','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Ενότητες/Δειγματικός χώρος - Ενδεχόμενα/Πίνακες/Pithanothtes-Table-Endexomena.tex','18/2/2021 11:35','-','','%# Στοιχεία αρχείου Pithanothtes-Table-Endexomena--------
\begin{longtable}{c>{\centering}m{2.5cm}>{\centering}m{5cm} c}
\hline \rule[-2ex]{0pt}{5.5ex} \textbf{Συμβολισμός} & \textbf{Ενδεχόμενο} & \textbf{Περιγραφή} & \textbf{Διάγραμμα} \\ 
\hhline{====} \rule[-2ex]{0pt}{8.5ex} $ x\in A $ & Ενδεχόμενο Α & Το ενδεχόμενο $ Α $ πραγματοποιείται. & \parbox[c]{22mm}{\mbox{}\\\begin{tikzpicture}[scale=.438]
\draw (-2,-2) rectangle (2.6,1);
\scope % A \cap B
\fill[red!80!black!30] (-.45,-.5) circle (1.1);
\draw[black] (-.45,-.5) circle (1.1);
\endscope
\tkzText(-1.6,-1.6){{\scriptsize $ \varOmega $}}
\tkzText(-.45,.1){{\scriptsize $ A $}}
\end{tikzpicture}} \\ 
\rule[-2ex]{0pt}{8.5ex} $ x\in A'' $ & Συμπλήρωμα του $ A $ & Το ενδεχόμενο $ A $ \textbf{δεν} πραγματοποιείται. & \parbox[c]{22mm}{\mbox{}\\\begin{tikzpicture}[scale=.438]
\filldraw[fill=red!80!black!30] (-2,-2) rectangle (2.6,1);
\scope % A \cap B
\fill[white] (-.45,-.5) circle (1.1);
\draw[black] (-.45,-.5) circle (1.1);
\endscope
\tkzText(-1.6,-1.6){{\scriptsize $ \varOmega $}}
\tkzText(-.45,.1){{\scriptsize $ A $}}
\end{tikzpicture}} \\ 
\rule[-2ex]{0pt}{8.5ex} $ x\in A\cup B $ & Ένωση του $ A $ με το $ B $ & Πραγματοποιείται ένα \textbf{τουλάχιστον} από τα ενδεχόμενα $ A $ και $ B $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillA \fillB
\end{venndiagram2sets}} \\ 
\rule[-2ex]{0pt}{8.5ex} $ x\in A\cap B $ & Τομή του $ A $ με το $ B $ & Πραγματοποιούνται \textbf{συγχρόνως} τα ενδ. $ A $ και $ B $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillACapB
\end{venndiagram2sets}} \\ 
\rule[-2ex]{0pt}{8.5ex} $ x\in A-B $ & Διαφορά του $ B $ απ'' το $ A $ & Πραγματοποιείται \textbf{μόνο} το ενδεχόμενο $ A $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillANotB
\end{venndiagram2sets}} \\ 
\rule[-2ex]{0pt}{8.5ex} $ x\in B-A $ & Διαφορά του $ A $ απ'' το $ B $ & Πραγματοποιείται \textbf{μόνο} το ενδεχόμενο $ B $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillBNotA
\end{venndiagram2sets}} \\ 
\rule[-2ex]{0pt}{8.5ex} $ x\in\left(A-B\right)\cup\left(B-A\right) $ & Ένωση διαφορών & Πραγματοποιείται \textbf{μόνο} ένα από τα δύο σύνολα (ή μόνο το $ A $ ή μόνο το $ B $). & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillANotB \fillBNotA
\end{venndiagram2sets}} \\ 
\rule[-2ex]{0pt}{8.5ex} \begin{minipage}{2.8cm}
\begin{center}
$ A\subseteq B $\\
$ x\in A\Rightarrow x\in B $
\end{center}
\end{minipage} & $ A $ υποσύνολο του $ Β $ & Η πραγματοποίηση του $ A $ συνεπάγεται πραγμ/ση του $ B $. & \parbox[c]{22mm}{\mbox{}\\\begin{tikzpicture}[scale=.438]
\draw (-2,-2) rectangle (2.6,1);
\scope % A \cap B
\filldraw[fill=red!80!black!30] (-.45,-.5) circle (1.1);
\draw[fill=red!80!black!30] (-.5,-.5) circle (.7);
\endscope
\tkzText(-1.6,-1.6){{\scriptsize $ \varOmega $}}
\tkzText(.9,.1){{\scriptsize $ B $}}
\tkzText(-.45,-.2){{\scriptsize $ A $}}
\end{tikzpicture}} \\ 
\rule[-2ex]{0pt}{8.5ex} $ x\in\left(A\cap B\right)'' $ & Συμπλήρωμα τομής & \textbf{Δεν} πραγματοποιούνται \textbf{συγχρονως} τα ενδ. $ A $ και $ B $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillNotAorNotB
\end{venndiagram2sets}}\\
\rule[-2ex]{0pt}{8.5ex} $ x\in\left(A\cup B\right)'' $ & Συμπλήρωμα ένωσης & Δεν πραγματοποιείται \textbf{κανένα} από τα ενδ. $ A $ και $ B $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillNotAorB
\end{venndiagram2sets}}\\
\rule[-2ex]{0pt}{8.5ex} $ x\in\left( A-B\right)''  $ & Συμπλήρωμα διαφοράς & \textbf{Δεν} πραγματοποιείται μόνο το ενδεχόμενο $ A $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillNotAorB \fillB
\end{venndiagram2sets}} \\
\rule[-2ex]{0pt}{8.5ex} $ x\in \left(B-A\right)''  $ & Συμπλήρωμα διαφοράς & \textbf{Δεν} πραγματοποιείται μόνο το ενδεχόμενο $ B $. & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillNotAorB \fillA
\end{venndiagram2sets}} \\
\rule[-2ex]{0pt}{8.5ex} $ x\in\left( \left(A-B\right)\cup\left(B-A\right)\right)''  $ & Συμπλήρωμα ένωσης διαφορών & \textbf{Δεν} πραγματοποιείται μόνο ένα από τα δύο σύνολα (ή κανένα από τα δύο ή και τα δύο). & \parbox[c]{22mm}{\mbox{}\\\begin{venndiagram2sets}[tikzoptions={scale=.4},shade=red!80!black!30,labelA={{\scriptsize $ A $}},labelB={{\scriptsize $ B $}},labelNotAB={{\scriptsize $ \varOmega $}}]
\fillNotAorB \fillACapB
\end{venndiagram2sets}} \\
\rule[-1ex]{0pt}{0ex} &&&\\
\hline
\end{longtable}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Table-IdiothtesDynamewn','Tab','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Δυνάμεις πραγματικών αριθμών/Πίνακες/Algebra-Table-IdiothtesDynamewn.tex','18/2/2021 12:43','','','%# Στοιχεία αρχείου Algebra-Table-IdiothtesDynamewn--------
\begin{longtable}{cc}
\hline \rule[-2ex]{0pt}{5.5ex}  \textbf{Ιδιότητα} & \textbf{Συνθήκη} \\
\hhline{==}\rule[-2ex]{0pt}{5.5ex}  Γινόμενο δυνάμεων με κοινή βάση & $ a^\nu\cdot a^\mu=a^{\nu+\mu} $ \\
\rule[-2ex]{0pt}{5.5ex}  Πηλίκο δυνάμεων με κοινή βάση & $ a^\nu: a^\mu=a^{\nu-\mu} $\\
\rule[-2ex]{0pt}{5.5ex}  Γινόμενο δυνάμεων με κοινό εκθέτη & $ \left(a\cdot\beta\right)^\nu=a^\nu\cdot\beta^\nu $ \\
\rule[-2ex]{0pt}{5.5ex} Πηλίκο δυνάμεων με κοινό εκθέτη & $ \left(\dfrac{a}{\beta}\right)^\nu=\dfrac{a^\nu}{\beta^\nu}\;\;,\;\;\beta\neq0 $ \\
\rule[-2ex]{0pt}{5.5ex}  Δύναμη υψωμένη σε δύναμη & $ \left( a^\nu\right)^\mu=a^{\nu\cdot\mu} $ \\
\rule[-2ex]{0pt}{5.5ex} Κλάσμα με αρνητικό εκθέτη & $ \left( \dfrac{a}{\beta}\right)^{-\nu}=\left(\dfrac{\beta}{a}\right)^\nu\;\;,\;\;a,\beta\neq0 $ \vspace{2mm}\\
\hline
\end{longtable}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Table-IdiothtesApolytwnTimwn','Tab','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Απόλυτη τιμή/Πίνακες/Algebra-Table-IdiothtesApolytwnTimwn.tex','18/2/2021 12:46','','','%# Στοιχεία αρχείου Algebra-Table-IdiothtesApolytwnTimwn--------
\begin{longtable}{cc}
\hline \rule[-2ex]{0pt}{5.5ex} \textbf{Ιδιότητα} & \textbf{Συνθήκη} \\
\hhline{==}\rule[-2ex]{0pt}{5.5ex} Πρόσημο απόλυτης τιμής & $ |a|=|-a|\geq0 $ \\
\rule[-2ex]{0pt}{5.5ex} Απόλυτη τιμή μηδενός & $ |a|=0\Leftrightarrow a=0 $\\
\rule[-2ex]{0pt}{5.5ex}  Όρια αριθμού & $ -|a|\leq a\leq|a| $ \\
\rule[-2ex]{0pt}{5.5ex}  Απόλυτη τιμή γινομένου & $ |a\cdot\beta|=|a|\cdot|\beta| $ \\
\rule[-2ex]{0pt}{5.5ex} Απόλυτη τιμή πηλίκου & $ \left| \dfrac{a}{\beta}\right|=\dfrac{|a|}{|\beta|} $ \\
\rule[-2ex]{0pt}{5.5ex} Τετράγωνο απόλυτης τιμής & $ |a|^2=a^2 $ \\
\rule[-2ex]{0pt}{5.5ex} Τριγωνική ανισότητα & $ \left||a-\beta| \right|\leq|a\pm\beta|\leq|a|+|\beta|  $ \\
\hline
\end{longtable}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Table-IdiothtesRizwn','Tab','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ρίζες πραγματικών αριθμών/Πίνακες/Algebra-Table-IdiothtesRizwn.tex','18/2/2021 12:51','','','%# Στοιχεία αρχείου Algebra-Table-IdiothtesRizwn--------
\begin{longtable}{cc}
\hline \rule[-2ex]{0pt}{5.5ex} \textbf{Ιδιότητα} & \textbf{Συνθήκη} \\
\hhline{==}\rule[-2ex]{0pt}{5.5ex} Τετράγωνο ρίζας & $ \left(\!\sqrt{x}\;\right)^2=x\;\;,\;\; x\geq0  $ \\
\rule[-2ex]{0pt}{5.5ex} Ν-οστή δύναμη ν-οστής ρίζας & $ \left(\!\sqrt[\nu]{x}\;\right)^\nu=x\;\;,\;\; x\geq0  $ \\
\rule[-2ex]{0pt}{5.5ex} Ρίζα τετραγώνου & $ \sqrt{x^2}=|x|\;\;,\;\; x\in\mathbb{R} $\\
\rule[-2ex]{0pt}{5.5ex} Ν-οστή ρίζα ν-οστής δύναμης & $ \sqrt[\nu]{x^\nu}=\begin{cases}
|x|&  x\in\mathbb{R}\textrm{ αν }\nu\textrm{ άρτιος}\\x&  x\geq0\textrm{ και } \nu\in\mathbb{N}\end{cases} $\\
\hhline{~-} \multirow{3}{*}{Ρίζα γινομένου} & $ \sqrt{x\cdot y}=\!\sqrt{x}\cdot\!\sqrt{y}\;\;,\;\; x,y\geq0 $ \rule[-2ex]{0pt}{5.5ex}\\
\rule[-2ex]{0pt}{5.5ex} & $ \sqrt[\nu]{x\cdot y}=\!\sqrt[\nu]{x}\cdot\!\sqrt[\nu]{y}\;\;,\;\; x,y\geq0 $ \\
\hhline{~-} \multirow{3}{*}{Ρίζα πηλίκου} & $ \sqrt{\dfrac{x}{y}}\;=\dfrac{\sqrt{x}}{\sqrt{y}}\;\;,\;\; x\geq0\textrm{ και }y>0 $ \rule[-2ex]{0pt}{6.5ex}\\
\rule[-2ex]{0pt}{7.5ex} & $ \sqrt[\nu]{\dfrac{x}{y}}\;=\dfrac{\sqrt[\nu]{x}}{\sqrt[\nu]{y}}\;\;,\;\; x\geq0\textrm{ και }y>0 $ \\
\hhline{~-}\rule[-2ex]{0pt}{5.5ex} Μ-οστή ρίζα ν-οστής ρίζας  & $ \sqrt[\mu]{\!\sqrt[\nu]{x}}=\!\sqrt[\nu\cdot\mu]{x}\;\;,\;\; x\geq0 $ \\
\rule[-2ex]{0pt}{5.5ex} Απλοποίηση ρίζας & $ \sqrt[\nu]{x^\nu\cdot y}=x\!\sqrt[\nu]{y}\;\;,\;\; x,y\geq0  $ \\
\rule[-2ex]{0pt}{5.5ex} Απλοποίηση τάξης και δύναμης & $ \sqrt[\mu\cdot\rho]{x^{\nu\cdot\rho}}=\!\sqrt[\mu]{x^{\nu}}\;\;,\;\; x\geq0 $ \\
\hline
\end{longtable}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Figure-Diagramma_DionymEx1','Fig','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Διωνυμικές εξισώσεις ν- οστού βαθμού/Σχήματα/Algebra-Figure-Diagramma_DionymEx1.tex','19/2/2021 00:49','','','%# Στοιχεία αρχείου Algebra-Figure-Diagramma_DionymEx1--------
\begin{tikzpicture}[box/.style={minimum height=.8cm,draw,rounded corners,minimum width=1.2cm,align=center},y=1.3cm]
\node[box] (eks) at (0,4) {{\footnotesize Αρχική εξίσωση}\\{\footnotesize $ x^\nu=a $}};
\node[box] (a) at (-2,3) {{\footnotesize $ \nu $ άρτιος}};
\node[box] (p) at (2,3) {{\footnotesize $ \nu $ περιττός}};
\node[box] (th1) at (-3,2) {{\footnotesize $ a\geq0 $}};
\node[box] (ar1) at (-1,2) {{\footnotesize $ a<0$}};
\node[box] (th2) at (1,2) {{\footnotesize $ a\geq0 $}};
\node[box] (ar2) at (3,2) {{\footnotesize $ a<0 $}};
\node[box] (ly1) at (-3,1) {{\footnotesize $ x=\pm \sqrt[\nu]{a} $}};
\node[box] (ly2) at (-1,1) {{\footnotesize αδύνατη}};
\node[box] (ly3) at (1,1) {{\footnotesize $ x=\sqrt[\nu]{a} $}};
\node[box] (ly4) at (3,1) {{\footnotesize $ x=-\!\sqrt[\nu]{|a|} $}};
\draw[-] (eks.270) -- (0,3.5);
\draw[-latex] (eks.270) -- (0,3.5) -- (-2,3.5) -- (a.90);
\draw[-latex] (0,3.5) -- (2,3.5) -- (p.90);
\draw[-latex] (a.270) -- (-2,2.5) -- (-3,2.5) -- (th1.90);
\draw[-latex] (-2,2.5) -- (-1,2.5) -- (ar1.90);
\draw[-latex] (p.270) -- (2,2.5) -- (1,2.5) -- (th2.90);
\draw[-latex] (2,2.5) -- (3,2.5) -- (ar2.90);
\draw[-latex] (th1.270)  -- (ly1.90);
\draw[-latex] (ar1.270)  -- (ly2.90);
\draw[-latex] (th2.270)  -- (ly3.90);
\draw[-latex] (ar2.270)  -- (ly4.90);
\node at (-6.4,4) {Αρχική εξίσωση};\draw[-latex] (-5,4)--(-4,4) ;
\node at (-7,3) {Εκθέτης};\draw[-latex] (-6.2,3)--(-4,3) ;
\node at (-6.7,2) {Αποτέλεσμα};\draw[-latex] (-5.6,2)--(-4,2) ;
\node (Eksiswsh) at (-7.1,1) {Λύσεις};\draw[-latex] (-6.3,1)--(-4,1) ;
\end{tikzpicture}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Table-EidhRizwn','Tab','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 2ου βαθμού/Πίνακες/Algebra-Table-EidhRizwn.tex','19/2/2021 14:23','','','%# Στοιχεία αρχείου Algebra-Table-EidhRizwn--------
\begin{longtable}{c|c|c|cc}
\hline \rule[-2ex]{0pt}{5.5ex} \boldmath$\varDelta$ & \boldmath$P$ & \boldmath$S$ & \textbf{Είδος λύσεων} & \textbf{Συμβολισμός}\\ 
\hhline{=====} \rule[-2ex]{0pt}{5.5ex}  &  & $ S>0 $ & Δύο θετικές πραγματικές & $ x_1>x_2>0 $ \\ 
\hhline{~|~-~~} \multirow{15}{*}{$ \varDelta>0 $}  & $ P>0 $ & $ S<0 $ & Δύο αρνητικές λύσεις & $ x_1<x_2<0 $ \rule[-2ex]{0pt}{5.5ex}\\ 
\hhline{~|~-~~}   &  & $ S=0 $ & \multicolumn{2}{c}{Αδύνατη περίπτωση}  \rule[-2ex]{0pt}{5.5ex}\\ 
\hhline{~|----}   &  & $ S>0 $ & \multirow{3}{*}{Ετερόσημες (όχι αντίθετες)} & $ x_1<0<x_2\;\;,\;\;|x_1|<|x_2| $ \rule[-2ex]{0pt}{5.5ex}\\ 
\hhline{~|~-~~} \rule[-2ex]{0pt}{5.5ex}  & $ P<0 $ & $ S<0 $ &  & $ x_1<0<x_2\;\;,\;\;|x_1|>|x_2| $ \\ 
\hhline{~|~-~~} \rule[-2ex]{0pt}{5.5ex}  &  & $ S=0 $ & Αντίθετες  & $ x_1=-x_2 $ \\ 
\hhline{~|----} \rule[-2ex]{0pt}{5.5ex}  &  & $ S>0 $ & Μηδενική και θετική & $ x_1=0\;\;,\;\;x_2>0 $ \\ 
\hhline{~|~-~~} \rule[-2ex]{0pt}{5.5ex}  & $ P=0 $ & $ S<0 $ & Μηδενική και αρνητική & $ x_1=0\;\;,\;\;x_2<0 $ \\ 
\hhline{~|~-~~} \rule[-2ex]{0pt}{5.5ex}  &  & $ S=0 $ &  \multicolumn{2}{c}{Αδύνατη περίπτωση}  \\ 
\hhline{~|----} \rule[-2ex]{0pt}{5.5ex}  & \multicolumn{2}{c|}{$ P=1 $} & Αντίστροφες & $ x_1=\frac{1}{x_2} $  \\ 
\hhline{-----}   & \multirow{3}{*}{$ P>0 $} & $ S>0 $ & Θετικές και ίσες  & $ x_1=x_2>0 $ \rule[-2ex]{0pt}{5.5ex}\\ 
\hhline{~|~|-|~~} \rule[-2ex]{0pt}{5.5ex} $ \varDelta=0 $ &  & $ S<0 $ & Αρνητικές και ίσες & $ x_1=x_2<0 $ \\ 
\hhline{~|--|~~} \rule[-2ex]{0pt}{5.5ex}  & $ P=0 $ & $ S=0 $ & Μηδενικές & $ x_1=x_2=0 $ \\ 
\hhline{-----} \rule[-2ex]{0pt}{5.5ex} $ \varDelta<0 $ & \multicolumn{4}{c}{Αδύνατη στο $ \mathbb{R} $}  \\ 
\hline 
\end{longtable}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',2021,'Α');
INSERT INTO "Database_Files" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx1','SectEx','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Μονοτονία ακρότατα συνάρτησης/Ασκήσεις/Μελέτη μονοτονίας με τη χρήση παραγώγου/Ana-Parag-MonotAkrot-MonotPar-SectEx1.tex','03/3/2021 18:28','ΟΧΙ','','%# Στοιχεία αρχείου Ana-Parag-MonotAkrot-MonotParSectEx1--------
Να μελετηθούν οι παρακάτω συναρτήσεις ως προς τη μονοτονία.
\begin{alist}
\item $ f(x)=x^2-2x $
\item $ f(x)=x^2-4x+3 $
\item $ f(x)=4-x^2 $
\item $ f(x)=4x^2-4x+1 $
\end{alist}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,'Γ΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx2','SectEx','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Μονοτονία ακρότατα συνάρτησης/Ασκήσεις/Μελέτη μονοτονίας με τη χρήση παραγώγου/Ana-Parag-MonotAkrot-MonotPar-SectEx2.tex','03/3/2021 18:38','ΟΧΙ','','%# Στοιχεία αρχείου Ana-Parag-MonotAkrot-MonotPar-SectEx2--------
Να μελετηθούν οι παρακάτω συναρτήσεις ως προς τη μονοτονία.
\begin{alist}
\item $ f(x)=\frac{x}{x^2-1} $
\item $ f(x)=\frac{x^2}{x^2+4} $
\item $ f(x)=\frac{x-1}{x+2} $
\item $ f(x)=\frac{x^2}{x-1} $
\end{alist}
%#--------------------------------------------------','Basic','PdfLaTeX',NULL,0,'Γ΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1','CombEx','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Συνδυαστικές ασκήσεις/Κυρτότητα - Σημεία καμπής-Μονοτονία ακρότατα συνάρτησης/Analysh-KyrtothtaMonotAkrot-CombEx1.tex','04/3/2021 12:16','YES','','%# Database File Analysh-KyrtothtaMonotAkrot-CombEx1
Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.
\begin{alist}
\item $ f(x)=\frac{x}{x^2+1} $
\item $ f(x)=\frac{x}{x^2-1} $
\item $ f(x)=\frac{x^2}{x^2-1} $
\item $ f(x)=\frac{x^2}{x-1} $
\item $ f(x)=\frac{x^3}{x-2} $
\item $ f(x)=\frac{1-x^2}{x^2+1} $
\end{alist}
%# End of file Analysh-KyrtothtaMonotAkrot-CombEx1','Basic','PdfLaTeX',NULL,1,'Γ΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2','CombEx','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Συνδυαστικές ασκήσεις/Κυρτότητα - Σημεία καμπής-Μονοτονία ακρότατα συνάρτησης/Analysh-KyrtothtaMonotAkrot-CombEx2.tex','04/3/2021 12:16','YES','','%# Database File Analysh-KyrtothtaMonotAkrot-CombEx2
Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.
\begin{alist}
\item $ f(x)=e^x-e\ln{x} $
\item $ f(x)=x\cdot\ln{x} $
\item $ f(x)=\dfrac{\ln{x}}{x} $
\item $ f(x)=\dfrac{e^x}{x} $
\item $ f(x)=\dfrac{x}{e^x} $
\item $ f(x)=\dfrac{x}{\ln{x}} $
\end{alist}
%# End of file Analysh-KyrtothtaMonotAkrot-CombEx2','Basic','PdfLaTeX',NULL,1,'Γ΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3','CombEx','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Συνδυαστικές ασκήσεις/Κυρτότητα - Σημεία καμπής-Μονοτονία ακρότατα συνάρτησης/Analysh-KyrtothtaMonotAkrot-CombEx3.tex','04/3/2021 12:29','YES','','%# Database File Analysh-KyrtothtaMonotAkrot-CombEx3
Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.
\begin{alist}
\item $ f(x)=\hm{x}+x\ \ ,\ \ x\in[0,2\pi] $
\item $ f(x)=\dfrac{\syn{x}}{e^x}\ \ ,\ \ x\in[-\pi,\pi] $
\item $ f(x)=\ln{\left(\hm{x}\right)}\ \ ,\ \ x\in(0,\pi) $
\end{alist}
%# End of file Analysh-KyrtothtaMonotAkrot-CombEx3','Basic','PdfLaTeX',NULL,1,'Γ΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Theorem-Proshmo_Triwnymou','Theor','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 2ου βαθμού/Θεωρήματα/Algebra-Theorem-Proshmo_Triwnymou.tex','11/3/2021 13:53','','','%# Database File : Algebra-Theorem-Proshmo_Triwnymou
Για το πρόσημο των τιμών ενός τριωνύμου $ ax^2+\beta x+\gamma $ ισχύουν οι παρακάτω κανόνες.
\begin{enumerate}[itemsep=0mm]
\item Αν η διακρίνουσα είναι θετική $\left( \varDelta>0\right)  $ τότε το τριώνυμο είναι
\begin{enumerate}[itemsep=0mm,label=\roman*.]
\item ομόσημο του συντελεστή $ a $ στα διαστήματα που βρίσκονται έξω από τις ρίζες $ x_1,x_2 $.
\item ετερόσημο του $ a $ στο διάστημα ανάμεσα στις ρίζες.
\item ίσο με το μηδέν στις ρίζες.
\end{enumerate}
\begin{center}
\begin{tikzpicture}
\tikzset{t style/.style = {style = dashed}}
\tkzTabInit[color,lgt=3,espcl=2,colorC = \xrwma!40,
colorL = \xrwma!20,
colorV = \xrwma!40]%
{$x$ / .8,$ax^2+\beta x+\gamma$ /1.2}%
{$-\infty$,$x_1$,$x_2$,$+\infty$}%
\tkzTabLine{ , \genfrac{}{}{0pt}{0}{\text{Ομόσημο}}{ \text{του } a}, z
, \genfrac{}{}{0pt}{0}{\text{Ετερόσημο}}{ \text{του } a}, z
, \genfrac{}{}{0pt}{0}{\text{Ομόσημο}}{ \text{του } a}, }
\end{tikzpicture}
\end{center}
\item Αν η διακρίνουσα είναι μηδενική $\left( \varDelta=0\right)  $ τότε το τριώνυμο είναι
\begin{enumerate}[itemsep=0mm,label=\roman*.]
\item ομόσημο του συντελεστή $ a $ στα διαστήματα που βρίσκονται δεξιά και αριστερά της ρίζας $ x_0 $.
\item ίσο με το μηδέν στη ρίζα.
\end{enumerate}
\begin{center}
\begin{tikzpicture}
\tikzset{t style/.style = {style = dashed}}
\tkzTabInit[color,lgt=3,espcl=2,colorC = \xrwma!40,
colorL = \xrwma!20,
colorV = \xrwma!40]%
{$x$ / .8,$ax^2+\beta x+\gamma$ /1.2}%
{$-\infty$,$x_0$,$+\infty$}%
\tkzTabLine{ , \genfrac{}{}{0pt}{0}{\text{Ομόσημο}}{ \text{του } a}, z
, \genfrac{}{}{0pt}{0}{\text{Ομόσημο}}{ \text{του } a}, }
\end{tikzpicture}
\end{center}
\item Αν η διακρίνουσα είναι αρνητική $\left( \varDelta<0\right)  $ τότε το τριώνυμο είναι
ομόσημο του συντελεστή $ a $ για κάθε $ x\in\mathbb{R}$.
\begin{center}
\begin{tikzpicture}
\tikzset{t style/.style = {style = dashed}}
\tkzTabInit[color,lgt=3,espcl=3.9,colorC = \xrwma!40,
colorL = \xrwma!20,
colorV = \xrwma!40]%
{$x$ / .8,$ax^2+\beta x+\gamma$ /1.2}%
{$-\infty$,$+\infty$}%
\tkzTabLine{, \genfrac{}{}{0pt}{0}{\text{Ομόσημο}}{ \text{του } a}, }
\end{tikzpicture}
\end{center}
\end{enumerate}\mbox{}\\
%# End of file Algebra-Theorem-Proshmo_Triwnymou','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Analysh-Table-Eidh_Synarthsewn_1','Tab','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ενότητες/Η έννοια της συνάρτησης/Πίνακες/Analysh-Table-Eidh_Synarthsewn_1.tex','11/3/2021 14:01','','','%# Database File : Analysh-Table-Eidh_Synarthsewn_1
\begin{longtable}{ccc}
\hline \rule[-2ex]{0pt}{5.5ex}\textbf{Είδος} & \textbf{Τύπος} & \textbf{Πεδίο Ορισμού} \\ 
\hhline{===} \rule[-2ex]{0pt}{5.5ex} \textbf{Πολυωνυμική} & $ f(x)=a_\nu x^\nu+\ldots+a_0 $ & $ D_f=\mathbb{R} $ \\
\rule[-2ex]{0pt}{5.5ex} \textbf{Ρητή} & $ f(x)=\dfrac{P(x)}{Q(x)} $ & $ D_f=\left\lbrace\left.  x\in\mathbb{R}\right| Q(x)\neq0\right\rbrace $  \\
\rule[-2ex]{0pt}{5.5ex} \textbf{Άρρητη} & $ f(x)=\sqrt{A(x)} $ & $ D_f=\left\lbrace\left. x\in\mathbb{R}\right| A(x)\geq0\right\rbrace $ \\ 
\hline 
\end{longtable}
%# End of file Analysh-Table-Eidh_Synarthsewn_1','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Algebra-Table-Typoi_Ar_Geom_Proodou','Tab','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ακολουθίες/Πίνακες/Algebra-Table-Typoi_Ar_Geom_Proodou.tex','17/3/2021 13:52','','','%# Database File : Algebra-Table-Typoi_Ar_Geom_Proodou
\begin{tabular}{ccc}
\hline \rule[-2ex]{0pt}{5.5ex}  & \textbf{Αριθμητική Πρόοδος} & \textbf{Γεωμετρική Πρόοδος} \\ 
\hhline{===} \rule[-2ex]{0pt}{5.5ex} \textbf{Αναδρομικός τύπος} & $ a_{\nu+1}=a_\nu+\omega $ & $ a_{\nu+1}=\lambda\cdot a_\nu $ \\ 
 \rule[-2ex]{0pt}{5.5ex} \textbf{Διαφορά / Λόγος} & $ \omega=a_{\nu+1}-a_\nu $ & $ \lambda=\frac{a_{\nu+1}}{a_\nu} $ \\ 
 \rule[-2ex]{0pt}{5.5ex} \textbf{Μέσος} & $ 2\beta=a+\gamma\;\;\textrm{ ή }\;\;\beta=\frac{a+\gamma}{2} $ & $ \beta^2=a\cdot\gamma $ \\ 
 \rule[-2ex]{0pt}{5.5ex} \textbf{Γενικός Όρος} & $ a_\nu=a_1+(\nu-1)\omega $ & $ a_\nu=a_1\cdot\lambda^{\nu-1} $ \\ 
\hhline{~--} \multirow{3}{*}{\textbf{Άθροισμα}} & $ S_\nu=\frac{\nu}{2}(a_1+a_\nu) $ & $ S_\nu=\dfrac{a_\nu\cdot\lambda-a_1}{\lambda-1} $  \rule[-2ex]{0pt}{7ex}\\ 
 \rule[-2ex]{0pt}{5.5ex}  & $ S_\nu=\frac{\nu}{2}\left[2a_1+(\nu-1)\omega\right]  $ & $ S_\nu=a_1\frac{\lambda^\nu-1}{\lambda-1} $ \\ 
\hline 
\end{tabular} 
%# End of file Algebra-Table-Typoi_Ar_Geom_Proodou','Basic','PdfLaTeX',NULL,0,'Α΄ Λυκείου',NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-AnisApT-SolSE4','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Λύσεις ασκήσεων ενότητας/Ανισώσεις με απόλυτες τιμές/Alg-Anis1ou-AnisApT-SolSE4.tex','19/3/2021 23:22','','',NULL,'Basic','PdfLaTeX',NULL,0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-Anis3ou+-Method-(Select a name here)','Method','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 3ου+ βαθμού - Γινόμενο/Μέθοδοι/Alg-Anis3ou+-Method-(Select a name here).tex','08/4/2021 00:34','','','\begin{center}
{\Large \textbf{Μονοτονία - ακρότατα - κυρτότητα - σημεία καμπής - \\σύνολο τιμών - πλήθος ριζών}}
\end{center}
\begin{enumerate}
%# Database File : Analysh-KyrtothtaMonotAkrot-CombEx1----
\item Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.
\begin{alist}
\item $ f(x)=\frac{x}{x^2+1} $
\item $ f(x)=\frac{x}{x^2-1} $
\item $ f(x)=\frac{x^2}{x^2-1} $
\item $ f(x)=\frac{x^2}{x-1} $
\item $ f(x)=\frac{x^3}{x-2} $
\item $ f(x)=\frac{1-x^2}{x^2+1} $
\end{alist}
%# End of file Analysh-KyrtothtaMonotAkrot-CombEx1

%# Database File : Analysh-KyrtothtaMonotAkrot-CombEx2----
\item Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.
\begin{alist}
\item $ f(x)=e^x-e\ln{x} $
\item $ f(x)=x\cdot\ln{x} $
\item $ f(x)=\dfrac{\ln{x}}{x} $
\item $ f(x)=\dfrac{e^x}{x} $
\item $ f(x)=\dfrac{x}{e^x} $
\item $ f(x)=\dfrac{x}{\ln{x}} $
\end{alist}
%# End of file Analysh-KyrtothtaMonotAkrot-CombEx2

%# Database File : Analysh-KyrtothtaMonotAkrot-CombEx3----
\item Για καθεμία από τις παρακάτω συναρτήσεις να βρείτε τα διαστήματα μονοτονίας, τα τοπικά ακρότατα, τα διαστήματα κυρτότητας καθώς και τα σημεία καμπής της γραφικής παράστασης. Στη συνέχεια να βρείτε το σύνολο τιμών κάθε συνάρτησης και το πλήθος ριζών της.
\begin{alist}
\item $ f(x)=\hm{x}+x\ \ ,\ \ x\in[0,2\pi] $
\item $ f(x)=\dfrac{\syn{x}}{e^x}\ \ ,\ \ x\in[-\pi,\pi] $
\item $ f(x)=\ln{\left(\hm{x}\right)}\ \ ,\ \ x\in(0,\pi) $
\end{alist}
%# End of file Analysh-KyrtothtaMonotAkrot-CombEx3

\end{enumerate}
','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','CombSub','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Συνδυαστικα θέματα/Αντίστροφη συνάρτηση-Η έννοια της συνάρτησης-Μονοτονία ακρότατα συνάρτησης-Συνάρτηση 1-1-Συνέχεια συνάρτησης/Θεωρία/DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1.tex','05/5/2021 18:44','ΟΧΙ','','%# Database File : DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1
\begin{thema}
\begin{erwthma}
\item Να αποδείξετε ότι αν μια συνάρτηση $ f $ είναι παραγωγίσιμη σε ένα σημείο $ x_0\in D_f $ τότε είναι και συνεχής στο σημείο αυτό.
\item Να δώσετε τον ορισμό της γνησίως αύξουσας συνάρτησης.
\item Να δώσετε τον ορισμό της $ 1-1 $ συνάρτησης.
\item Να χαρακτηρίσετε καθεμία από τις ακόλουθες προτάσεις ως σωστή (Σωστό) ή λανθασμένη (Λάθος).
\begin{alist}
\item Δύο συναρτήσεις $ f,g $ είναι ίσες αν ισχύει $ f(x)=g(x) $ για κάθε $ x\in D_f\cap D_g $.
\item Αν για κάθε $ x\in D_f $ ισχύει η σχέση $ f(x)\geq 2 $ τότε το $ 2 $ είναι ολικό ελάχιστο της $ f $.
\item 
\item Για οποιεσδήποτε συναρτήσεις $ f,g,h $ ισχύει η ισότητα $ (f\circ g)\circ h=f\circ(g\circ h) $.
\item Η συνάρτηση $ f(x)=\frac{1}{x} $ είναι γνησίως φθίνουσα στο $ \mathbb{R}^* $.
\end{alist}
\end{erwthma}
\end{thema}
%# End of file DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Basic','PdfLaTeX',NULL,1,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','CombSub','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Συνδυαστικα θέματα/Γραφική παράσταση συνάρτησης-Θεώρημα ενδιάμεσων τιμών - Θεώρημα μέγιστης και ελάχιστης τιμής-Σύνθεση συναρτήσεων/Θεωρία/DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1.tex','06/5/2021 11:59','ΟΧΙ','','%# Database File : DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1
\begin{erwthma}
\item Να δώσετε τον ορισμό της γραφικής παράστασης μιας συνάρτησης $ f $.
\item Να διατυπώσετε και να αποδείξετε το θεώρημα ενδιάμεσων τιμών.
\end{erwthma}
Στο παρακάτω σχήμα βλέπουμε τις γραφικές παραστάσεις δύο συναρτήσεων $ f,g $.
\definecolor{ccqqqq}{rgb}{0.8,0,0}
\definecolor{qqwuqq}{rgb}{0,0.39215686274509803,0}
\begin{center}
\begin{tikzpicture}[line cap=round,line join=round,>=triangle 45,x=1cm,y=1cm]
\begin{axis}[
x=1cm,y=.7cm,
axis lines=middle,
ymajorgrids=true,
xmajorgrids=true,
xmin=-1.299999999999999,
xmax=8.26000000000001,
ymin=-5.079999999999998,
ymax=7.099999999999997,
xtick={-3,-2,...,16},
ytick={-5,-4,...,7},]
\clip(-3.3,-5.38) rectangle (8.26,7.5);
\draw[line width=2pt,color=qqwuqq] (1.0000001796000004,-3.999998922400028) -- (1.0000001796000004,-3.999998922400028);
\draw[line width=2pt,color=qqwuqq] (1.0000001796000004,-3.999998922400028) -- (1.0150001791619605,-3.910223930403127);
\draw[line width=2pt,color=qqwuqq] (1.0150001791619605,-3.910223930403127) -- (1.0300001787239206,-3.820898938379946);
\draw[line width=2pt,color=qqwuqq] (1.0300001787239206,-3.820898938379946) -- (1.0450001782858807,-3.732023946330477);
\draw[line width=2pt,color=qqwuqq] (1.0450001782858807,-3.732023946330477) -- (1.0600001778478407,-3.6435989542547276);
\draw[line width=2pt,color=qqwuqq] (1.0600001778478407,-3.6435989542547276) -- (1.0750001774098008,-3.555623962152696);
\draw[line width=2pt,color=qqwuqq] (1.0750001774098008,-3.555623962152696) -- (1.0900001769717609,-3.468098970024384);
\draw[line width=2pt,color=qqwuqq] (1.0900001769717609,-3.468098970024384) -- (1.105000176533721,-3.3810239778697877);
\draw[line width=2pt,color=qqwuqq] (1.105000176533721,-3.3810239778697877) -- (1.120000176095681,-3.2943989856889075);
\draw[line width=2pt,color=qqwuqq] (1.120000176095681,-3.2943989856889075) -- (1.135000175657641,-3.208223993481747);
\draw[line width=2pt,color=qqwuqq] (1.135000175657641,-3.208223993481747) -- (1.1500001752196012,-3.122499001248306);
\draw[line width=2pt,color=qqwuqq] (1.1500001752196012,-3.122499001248306) -- (1.1650001747815613,-3.037224008988579);
\draw[line width=2pt,color=qqwuqq] (1.1650001747815613,-3.037224008988579) -- (1.1800001743435213,-2.952399016702569);
\draw[line width=2pt,color=qqwuqq] (1.1800001743435213,-2.952399016702569) -- (1.1950001739054814,-2.8680240243902793);
\draw[line width=2pt,color=qqwuqq] (1.1950001739054814,-2.8680240243902793) -- (1.2100001734674415,-2.7840990320517083);
\draw[line width=2pt,color=qqwuqq] (1.2100001734674415,-2.7840990320517083) -- (1.2250001730294016,-2.7006240396868515);
\draw[line width=2pt,color=qqwuqq] (1.2250001730294016,-2.7006240396868515) -- (1.2400001725913616,-2.6175990472957125);
\draw[line width=2pt,color=qqwuqq] (1.2400001725913616,-2.6175990472957125) -- (1.2550001721533217,-2.535024054878294);
\draw[line width=2pt,color=qqwuqq] (1.2550001721533217,-2.535024054878294) -- (1.2700001717152818,-2.452899062434592);
\draw[line width=2pt,color=qqwuqq] (1.2700001717152818,-2.452899062434592) -- (1.2850001712772419,-2.3712240699646063);
\draw[line width=2pt,color=qqwuqq] (1.2850001712772419,-2.3712240699646063) -- (1.300000170839202,-2.2899990774683374);
\draw[line width=2pt,color=qqwuqq] (1.300000170839202,-2.2899990774683374) -- (1.315000170401162,-2.209224084945789);
\draw[line width=2pt,color=qqwuqq] (1.315000170401162,-2.209224084945789) -- (1.330000169963122,-2.128899092396958);
\draw[line width=2pt,color=qqwuqq] (1.330000169963122,-2.128899092396958) -- (1.3450001695250822,-2.0490240998218425);
\draw[line width=2pt,color=qqwuqq] (1.3450001695250822,-2.0490240998218425) -- (1.3600001690870422,-1.9695991072204446);
\draw[line width=2pt,color=qqwuqq] (1.3600001690870422,-1.9695991072204446) -- (1.3750001686490023,-1.8906241145927662);
\draw[line width=2pt,color=qqwuqq] (1.3750001686490023,-1.8906241145927662) -- (1.3900001682109624,-1.8120991219388056);
\draw[line width=2pt,color=qqwuqq] (1.3900001682109624,-1.8120991219388056) -- (1.4050001677729225,-1.73402412925856);
\draw[line width=2pt,color=qqwuqq] (1.4050001677729225,-1.73402412925856) -- (1.4200001673348825,-1.6563991365520332);
\draw[line width=2pt,color=qqwuqq] (1.4200001673348825,-1.6563991365520332) -- (1.4350001668968426,-1.579224143819225);
\draw[line width=2pt,color=qqwuqq] (1.4350001668968426,-1.579224143819225) -- (1.4500001664588027,-1.5024991510601353);
\draw[line width=2pt,color=qqwuqq] (1.4500001664588027,-1.5024991510601353) -- (1.4650001660207628,-1.42622415827476);
\draw[line width=2pt,color=qqwuqq] (1.4650001660207628,-1.42622415827476) -- (1.4800001655827228,-1.3503991654631031);
\draw[line width=2pt,color=qqwuqq] (1.4800001655827228,-1.3503991654631031) -- (1.495000165144683,-1.2750241726251659);
\draw[line width=2pt,color=qqwuqq] (1.495000165144683,-1.2750241726251659) -- (1.510000164706643,-1.2000991797609464);
\draw[line width=2pt,color=qqwuqq] (1.510000164706643,-1.2000991797609464) -- (1.525000164268603,-1.125624186870442);
\draw[line width=2pt,color=qqwuqq] (1.525000164268603,-1.125624186870442) -- (1.5400001638305632,-1.0515991939536553);
\draw[line width=2pt,color=qqwuqq] (1.5400001638305632,-1.0515991939536553) -- (1.5550001633925232,-0.9780242010105882);
\draw[line width=2pt,color=qqwuqq] (1.5550001633925232,-0.9780242010105882) -- (1.5700001629544833,-0.9048992080412388);
\draw[line width=2pt,color=qqwuqq] (1.5700001629544833,-0.9048992080412388) -- (1.5850001625164434,-0.8322242150456045);
\draw[line width=2pt,color=qqwuqq] (1.5850001625164434,-0.8322242150456045) -- (1.6000001620784035,-0.7599992220236889);
\draw[line width=2pt,color=qqwuqq] (1.6000001620784035,-0.7599992220236889) -- (1.6150001616403635,-0.6882242289754918);
\draw[line width=2pt,color=qqwuqq] (1.6150001616403635,-0.6882242289754918) -- (1.6300001612023236,-0.6168992359010135);
\draw[line width=2pt,color=qqwuqq] (1.6300001612023236,-0.6168992359010135) -- (1.6450001607642837,-0.5460242428002493);
\draw[line width=2pt,color=qqwuqq] (1.6450001607642837,-0.5460242428002493) -- (1.6600001603262438,-0.47559924967320377);
\draw[line width=2pt,color=qqwuqq] (1.6600001603262438,-0.47559924967320377) -- (1.6750001598882038,-0.40562425651987777);
\draw[line width=2pt,color=qqwuqq] (1.6750001598882038,-0.40562425651987777) -- (1.690000159450164,-0.3360992633402695);
\draw[line width=2pt,color=qqwuqq] (1.690000159450164,-0.3360992633402695) -- (1.705000159012124,-0.26702427013437635);
\draw[line width=2pt,color=qqwuqq] (1.705000159012124,-0.26702427013437635) -- (1.720000158574084,-0.19839927690220094);
\draw[line width=2pt,color=qqwuqq] (1.720000158574084,-0.19839927690220094) -- (1.7350001581360441,-0.13022428364374505);
\draw[line width=2pt,color=qqwuqq] (1.7350001581360441,-0.13022428364374505) -- (1.7500001576980042,-0.06249929035900692);
\draw[line width=2pt,color=qqwuqq] (1.7500001576980042,-0.06249929035900692) -- (1.7650001572599643,0.004775702952015237);
\draw[line width=2pt,color=qqwuqq] (1.7650001572599643,0.004775702952015237) -- (1.7800001568219244,0.07160069628932053);
\draw[line width=2pt,color=qqwuqq] (1.7800001568219244,0.07160069628932053) -- (1.7950001563838844,0.1379756896529063);
\draw[line width=2pt,color=qqwuqq] (1.7950001563838844,0.1379756896529063) -- (1.8100001559458445,0.20390068304277342);
\draw[line width=2pt,color=qqwuqq] (1.8100001559458445,0.20390068304277342) -- (1.8250001555078046,0.26937567645892546);
\draw[line width=2pt,color=qqwuqq] (1.8250001555078046,0.26937567645892546) -- (1.8400001550697647,0.33440066990136064);
\draw[line width=2pt,color=qqwuqq] (1.8400001550697647,0.33440066990136064) -- (1.8550001546317247,0.3989756633700754);
\draw[line width=2pt,color=qqwuqq] (1.8550001546317247,0.3989756633700754) -- (1.8700001541936848,0.4631006568650724);
\draw[line width=2pt,color=qqwuqq] (1.8700001541936848,0.4631006568650724) -- (1.885000153755645,0.5267756503863543);
\draw[line width=2pt,color=qqwuqq] (1.885000153755645,0.5267756503863543) -- (1.900000153317605,0.5900006439339185);
\draw[line width=2pt,color=qqwuqq] (1.900000153317605,0.5900006439339185) -- (1.915000152879565,0.6527756375077631);
\draw[line width=2pt,color=qqwuqq] (1.915000152879565,0.6527756375077631) -- (1.9300001524415251,0.71510063110789);
\draw[line width=2pt,color=qqwuqq] (1.9300001524415251,0.71510063110789) -- (1.9450001520034852,0.7769756247343009);
\draw[line width=2pt,color=qqwuqq] (1.9450001520034852,0.7769756247343009) -- (1.9600001515654453,0.838400618386995);
\draw[line width=2pt,color=qqwuqq] (1.9600001515654453,0.838400618386995) -- (1.9750001511274053,0.8993756120659686);
\draw[line width=2pt,color=qqwuqq] (1.9750001511274053,0.8993756120659686) -- (1.9900001506893654,0.9599006057712254);
\draw[line width=2pt,color=qqwuqq] (1.9900001506893654,0.9599006057712254) -- (2.0050001502513255,1.0199755995027662);
\draw[line width=2pt,color=qqwuqq] (2.0050001502513255,1.0199755995027662) -- (2.0200001498132854,1.0796005932605874);
\draw[line width=2pt,color=qqwuqq] (2.0200001498132854,1.0796005932605874) -- (2.035000149375245,1.1387755870446914);
\draw[line width=2pt,color=qqwuqq] (2.035000149375245,1.1387755870446914) -- (2.050000148937205,1.1975005808550776);
\draw[line width=2pt,color=qqwuqq] (2.050000148937205,1.1975005808550776) -- (2.065000148499165,1.255775574691746);
\draw[line width=2pt,color=qqwuqq] (2.065000148499165,1.255775574691746) -- (2.0800001480611248,1.3136005685546972);
\draw[line width=2pt,color=qqwuqq] (2.0800001480611248,1.3136005685546972) -- (2.0950001476230846,1.3709755624439306);
\draw[line width=2pt,color=qqwuqq] (2.0950001476230846,1.3709755624439306) -- (2.1100001471850445,1.4279005563594462);
\draw[line width=2pt,color=qqwuqq] (2.1100001471850445,1.4279005563594462) -- (2.1250001467470043,1.4843755503012446);
\draw[line width=2pt,color=qqwuqq] (2.1250001467470043,1.4843755503012446) -- (2.140000146308964,1.5404005442693252);
\draw[line width=2pt,color=qqwuqq] (2.140000146308964,1.5404005442693252) -- (2.155000145870924,1.5959755382636884);
\draw[line width=2pt,color=qqwuqq] (2.155000145870924,1.5959755382636884) -- (2.170000145432884,1.651100532284334);
\draw[line width=2pt,color=qqwuqq] (2.170000145432884,1.651100532284334) -- (2.1850001449948437,1.7057755263312617);
\draw[line width=2pt,color=qqwuqq] (2.1850001449948437,1.7057755263312617) -- (2.2000001445568036,1.7600005204044722);
\draw[line width=2pt,color=qqwuqq] (2.2000001445568036,1.7600005204044722) -- (2.2150001441187634,1.813775514503965);
\draw[line width=2pt,color=qqwuqq] (2.2150001441187634,1.813775514503965) -- (2.2300001436807233,1.8671005086297399);
\draw[line width=2pt,color=qqwuqq] (2.2300001436807233,1.8671005086297399) -- (2.245000143242683,1.9199755027817975);
\draw[line width=2pt,color=qqwuqq] (2.245000143242683,1.9199755027817975) -- (2.260000142804643,1.9724004969601374);
\draw[line width=2pt,color=qqwuqq] (2.260000142804643,1.9724004969601374) -- (2.275000142366603,2.0243754911647596);
\draw[line width=2pt,color=qqwuqq] (2.275000142366603,2.0243754911647596) -- (2.2900001419285627,2.0759004853956644);
\draw[line width=2pt,color=qqwuqq] (2.2900001419285627,2.0759004853956644) -- (2.3050001414905226,2.1269754796528515);
\draw[line width=2pt,color=qqwuqq] (2.3050001414905226,2.1269754796528515) -- (2.3200001410524824,2.177600473936321);
\draw[line width=2pt,color=qqwuqq] (2.3200001410524824,2.177600473936321) -- (2.3350001406144423,2.227775468246073);
\draw[line width=2pt,color=qqwuqq] (2.3350001406144423,2.227775468246073) -- (2.350000140176402,2.2775004625821076);
\draw[line width=2pt,color=qqwuqq] (2.350000140176402,2.2775004625821076) -- (2.365000139738362,2.326775456944424);
\draw[line width=2pt,color=qqwuqq] (2.365000139738362,2.326775456944424) -- (2.380000139300322,2.3756004513330233);
\draw[line width=2pt,color=qqwuqq] (2.380000139300322,2.3756004513330233) -- (2.3950001388622817,2.423975445747905);
\draw[line width=2pt,color=qqwuqq] (2.3950001388622817,2.423975445747905) -- (2.4100001384242415,2.471900440189069);
\draw[line width=2pt,color=qqwuqq] (2.4100001384242415,2.471900440189069) -- (2.4250001379862014,2.5193754346565154);
\draw[line width=2pt,color=qqwuqq] (2.4250001379862014,2.5193754346565154) -- (2.4400001375481613,2.566400429150244);
\draw[line width=2pt,color=qqwuqq] (2.4400001375481613,2.566400429150244) -- (2.455000137110121,2.6129754236702554);
\draw[line width=2pt,color=qqwuqq] (2.455000137110121,2.6129754236702554) -- (2.470000136672081,2.659100418216549);
\draw[line width=2pt,color=qqwuqq] (2.470000136672081,2.659100418216549) -- (2.485000136234041,2.7047754127891253);
\draw[line width=2pt,color=qqwuqq] (2.485000136234041,2.7047754127891253) -- (2.5000001357960007,2.7500004073879833);
\draw[line width=2pt,color=qqwuqq] (2.5000001357960007,2.7500004073879833) -- (2.5150001353579605,2.7947754020131246);
\draw[line width=2pt,color=qqwuqq] (2.5150001353579605,2.7947754020131246) -- (2.5300001349199204,2.8391003966645476);
\draw[line width=2pt,color=qqwuqq] (2.5300001349199204,2.8391003966645476) -- (2.5450001344818802,2.8829753913422533);
\draw[line width=2pt,color=qqwuqq] (2.5450001344818802,2.8829753913422533) -- (2.56000013404384,2.9264003860462413);
\draw[line width=2pt,color=qqwuqq] (2.56000013404384,2.9264003860462413) -- (2.5750001336058,2.969375380776512);
\draw[line width=2pt,color=qqwuqq] (2.5750001336058,2.969375380776512) -- (2.59000013316776,3.011900375533065);
\draw[line width=2pt,color=qqwuqq] (2.59000013316776,3.011900375533065) -- (2.6050001327297196,3.0539753703159);
\draw[line width=2pt,color=qqwuqq] (2.6050001327297196,3.0539753703159) -- (2.6200001322916795,3.095600365125018);
\draw[line width=2pt,color=qqwuqq] (2.6200001322916795,3.095600365125018) -- (2.6350001318536393,3.136775359960418);
\draw[line width=2pt,color=qqwuqq] (2.6350001318536393,3.136775359960418) -- (2.650000131415599,3.177500354822101);
\draw[line width=2pt,color=qqwuqq] (2.650000131415599,3.177500354822101) -- (2.665000130977559,3.217775349710066);
\draw[line width=2pt,color=qqwuqq] (2.665000130977559,3.217775349710066) -- (2.680000130539519,3.257600344624313);
\draw[line width=2pt,color=qqwuqq] (2.680000130539519,3.257600344624313) -- (2.6950001301014788,3.2969753395648427);
\draw[line width=2pt,color=qqwuqq] (2.6950001301014788,3.2969753395648427) -- (2.7100001296634386,3.335900334531655);
\draw[line width=2pt,color=qqwuqq] (2.7100001296634386,3.335900334531655) -- (2.7250001292253985,3.3743753295247494);
\draw[line width=2pt,color=qqwuqq] (2.7250001292253985,3.3743753295247494) -- (2.7400001287873583,3.4124003245441266);
\draw[line width=2pt,color=qqwuqq] (2.7400001287873583,3.4124003245441266) -- (2.755000128349318,3.449975319589786);
\draw[line width=2pt,color=qqwuqq] (2.755000128349318,3.449975319589786) -- (2.770000127911278,3.4871003146617277);
\draw[line width=2pt,color=qqwuqq] (2.770000127911278,3.4871003146617277) -- (2.785000127473238,3.5237753097599516);
\draw[line width=2pt,color=qqwuqq] (2.785000127473238,3.5237753097599516) -- (2.8000001270351977,3.5600003048844586);
\draw[line width=2pt,color=qqwuqq] (2.8000001270351977,3.5600003048844586) -- (2.8150001265971576,3.5957753000352475);
\draw[line width=2pt,color=qqwuqq] (2.8150001265971576,3.5957753000352475) -- (2.8300001261591174,3.6311002952123186);
\draw[line width=2pt,color=qqwuqq] (2.8300001261591174,3.6311002952123186) -- (2.8450001257210773,3.665975290415673);
\draw[line width=2pt,color=qqwuqq] (2.8450001257210773,3.665975290415673) -- (2.860000125283037,3.7004002856453093);
\draw[line width=2pt,color=qqwuqq] (2.860000125283037,3.7004002856453093) -- (2.875000124844997,3.7343752809012276);
\draw[line width=2pt,color=qqwuqq] (2.875000124844997,3.7343752809012276) -- (2.890000124406957,3.767900276183429);
\draw[line width=2pt,color=qqwuqq] (2.890000124406957,3.767900276183429) -- (2.9050001239689167,3.8009752714919123);
\draw[line width=2pt,color=qqwuqq] (2.9050001239689167,3.8009752714919123) -- (2.9200001235308766,3.833600266826678);
\draw[line width=2pt,color=qqwuqq] (2.9200001235308766,3.833600266826678) -- (2.9350001230928364,3.8657752621877264);
\draw[line width=2pt,color=qqwuqq] (2.9350001230928364,3.8657752621877264) -- (2.9500001226547963,3.8975002575750572);
\draw[line width=2pt,color=qqwuqq] (2.9500001226547963,3.8975002575750572) -- (2.965000122216756,3.92877525298867);
\draw[line width=2pt,color=qqwuqq] (2.965000122216756,3.92877525298867) -- (2.980000121778716,3.9596002484285657);
\draw[line width=2pt,color=qqwuqq] (2.980000121778716,3.9596002484285657) -- (2.995000121340676,3.989975243894744);
\draw[line width=2pt,color=qqwuqq] (2.995000121340676,3.989975243894744) -- (3.0100001209026357,4.019900239387204);
\draw[line width=2pt,color=qqwuqq] (3.0100001209026357,4.019900239387204) -- (3.0250001204645955,4.049375234905947);
\draw[line width=2pt,color=qqwuqq] (3.0250001204645955,4.049375234905947) -- (3.0400001200265554,4.078400230450972);
\draw[line width=2pt,color=qqwuqq] (3.0400001200265554,4.078400230450972) -- (3.0550001195885153,4.106975226022279);
\draw[line width=2pt,color=qqwuqq] (3.0550001195885153,4.106975226022279) -- (3.070000119150475,4.13510022161987);
\draw[line width=2pt,color=qqwuqq] (3.070000119150475,4.13510022161987) -- (3.085000118712435,4.162775217243742);
\draw[line width=2pt,color=qqwuqq] (3.085000118712435,4.162775217243742) -- (3.100000118274395,4.190000212893897);
\draw[line width=2pt,color=qqwuqq] (3.100000118274395,4.190000212893897) -- (3.1150001178363547,4.216775208570334);
\draw[line width=2pt,color=qqwuqq] (3.1150001178363547,4.216775208570334) -- (3.1300001173983145,4.243100204273054);
\draw[line width=2pt,color=qqwuqq] (3.1300001173983145,4.243100204273054) -- (3.1450001169602744,4.268975200002055);
\draw[line width=2pt,color=qqwuqq] (3.1450001169602744,4.268975200002055) -- (3.1600001165222342,4.29440019575734);
\draw[line width=2pt,color=qqwuqq] (3.1600001165222342,4.29440019575734) -- (3.175000116084194,4.3193751915389065);
\draw[line width=2pt,color=qqwuqq] (3.175000116084194,4.3193751915389065) -- (3.190000115646154,4.343900187346756);
\draw[line width=2pt,color=qqwuqq] (3.190000115646154,4.343900187346756) -- (3.205000115208114,4.367975183180888);
\draw[line width=2pt,color=qqwuqq] (3.205000115208114,4.367975183180888) -- (3.2200001147700736,4.391600179041301);
\draw[line width=2pt,color=qqwuqq] (3.2200001147700736,4.391600179041301) -- (3.2350001143320335,4.414775174927998);
\draw[line width=2pt,color=qqwuqq] (3.2350001143320335,4.414775174927998) -- (3.2500001138939933,4.437500170840977);
\draw[line width=2pt,color=qqwuqq] (3.2500001138939933,4.437500170840977) -- (3.265000113455953,4.459775166780238);
\draw[line width=2pt,color=qqwuqq] (3.265000113455953,4.459775166780238) -- (3.280000113017913,4.4816001627457815);
\draw[line width=2pt,color=qqwuqq] (3.280000113017913,4.4816001627457815) -- (3.295000112579873,4.502975158737608);
\draw[line width=2pt,color=qqwuqq] (3.295000112579873,4.502975158737608) -- (3.3100001121418328,4.523900154755717);
\draw[line width=2pt,color=qqwuqq] (3.3100001121418328,4.523900154755717) -- (3.3250001117037926,4.544375150800107);
\draw[line width=2pt,color=qqwuqq] (3.3250001117037926,4.544375150800107) -- (3.3400001112657525,4.564400146870781);
\draw[line width=2pt,color=qqwuqq] (3.3400001112657525,4.564400146870781) -- (3.3550001108277123,4.583975142967737);
\draw[line width=2pt,color=qqwuqq] (3.3550001108277123,4.583975142967737) -- (3.370000110389672,4.603100139090975);
\draw[line width=2pt,color=qqwuqq] (3.370000110389672,4.603100139090975) -- (3.385000109951632,4.621775135240496);
\draw[line width=2pt,color=qqwuqq] (3.385000109951632,4.621775135240496) -- (3.400000109513592,4.640000131416298);
\draw[line width=2pt,color=qqwuqq] (3.400000109513592,4.640000131416298) -- (3.4150001090755517,4.657775127618383);
\draw[line width=2pt,color=qqwuqq] (3.4150001090755517,4.657775127618383) -- (3.4300001086375116,4.675100123846751);
\draw[line width=2pt,color=qqwuqq] (3.4300001086375116,4.675100123846751) -- (3.4450001081994714,4.691975120101402);
\draw[line width=2pt,color=qqwuqq] (3.4450001081994714,4.691975120101402) -- (3.4600001077614313,4.708400116382334);
\draw[line width=2pt,color=qqwuqq] (3.4600001077614313,4.708400116382334) -- (3.475000107323391,4.7243751126895495);
\draw[line width=2pt,color=qqwuqq] (3.475000107323391,4.7243751126895495) -- (3.490000106885351,4.739900109023047);
\draw[line width=2pt,color=qqwuqq] (3.490000106885351,4.739900109023047) -- (3.505000106447311,4.754975105382826);
\draw[line width=2pt,color=qqwuqq] (3.505000106447311,4.754975105382826) -- (3.5200001060092707,4.769600101768889);
\draw[line width=2pt,color=qqwuqq] (3.5200001060092707,4.769600101768889) -- (3.5350001055712306,4.783775098181233);
\draw[line width=2pt,color=qqwuqq] (3.5350001055712306,4.783775098181233) -- (3.5500001051331904,4.797500094619861);
\draw[line width=2pt,color=qqwuqq] (3.5500001051331904,4.797500094619861) -- (3.5650001046951503,4.8107750910847695);
\draw[line width=2pt,color=qqwuqq] (3.5650001046951503,4.8107750910847695) -- (3.58000010425711,4.8236000875759615);
\draw[line width=2pt,color=qqwuqq] (3.58000010425711,4.8236000875759615) -- (3.59500010381907,4.835975084093436);
\draw[line width=2pt,color=qqwuqq] (3.59500010381907,4.835975084093436) -- (3.61000010338103,4.847900080637192);
\draw[line width=2pt,color=qqwuqq] (3.61000010338103,4.847900080637192) -- (3.6250001029429897,4.859375077207232);
\draw[line width=2pt,color=qqwuqq] (3.6250001029429897,4.859375077207232) -- (3.6400001025049495,4.8704000738035536);
\draw[line width=2pt,color=qqwuqq] (3.6400001025049495,4.8704000738035536) -- (3.6550001020669094,4.880975070426157);
\draw[line width=2pt,color=qqwuqq] (3.6550001020669094,4.880975070426157) -- (3.6700001016288692,4.891100067075043);
\draw[line width=2pt,color=qqwuqq] (3.6700001016288692,4.891100067075043) -- (3.685000101190829,4.900775063750212);
\draw[line width=2pt,color=qqwuqq] (3.685000101190829,4.900775063750212) -- (3.700000100752789,4.910000060451663);
\draw[line width=2pt,color=qqwuqq] (3.700000100752789,4.910000060451663) -- (3.715000100314749,4.9187750571793964);
\draw[line width=2pt,color=qqwuqq] (3.715000100314749,4.9187750571793964) -- (3.7300000998767087,4.927100053933413);
\draw[line width=2pt,color=qqwuqq] (3.7300000998767087,4.927100053933413) -- (3.7450000994386685,4.934975050713711);
\draw[line width=2pt,color=qqwuqq] (3.7450000994386685,4.934975050713711) -- (3.7600000990006284,4.942400047520292);
\draw[line width=2pt,color=qqwuqq] (3.7600000990006284,4.942400047520292) -- (3.7750000985625882,4.949375044353155);
\draw[line width=2pt,color=qqwuqq] (3.7750000985625882,4.949375044353155) -- (3.790000098124548,4.955900041212301);
\draw[line width=2pt,color=qqwuqq] (3.790000098124548,4.955900041212301) -- (3.805000097686508,4.961975038097728);
\draw[line width=2pt,color=qqwuqq] (3.805000097686508,4.961975038097728) -- (3.820000097248468,4.967600035009439);
\draw[line width=2pt,color=qqwuqq] (3.820000097248468,4.967600035009439) -- (3.8350000968104276,4.972775031947432);
\draw[line width=2pt,color=qqwuqq] (3.8350000968104276,4.972775031947432) -- (3.8500000963723875,4.977500028911707);
\draw[line width=2pt,color=qqwuqq] (3.8500000963723875,4.977500028911707) -- (3.8650000959343473,4.981775025902264);
\draw[line width=2pt,color=qqwuqq] (3.8650000959343473,4.981775025902264) -- (3.880000095496307,4.985600022919105);
\draw[line width=2pt,color=qqwuqq] (3.880000095496307,4.985600022919105) -- (3.895000095058267,4.988975019962227);
\draw[line width=2pt,color=qqwuqq] (3.895000095058267,4.988975019962227) -- (3.910000094620227,4.991900017031632);
\draw[line width=2pt,color=qqwuqq] (3.910000094620227,4.991900017031632) -- (3.9250000941821868,4.9943750141273195);
\draw[line width=2pt,color=qqwuqq] (3.9250000941821868,4.9943750141273195) -- (3.9400000937441466,4.996400011249289);
\draw[line width=2pt,color=qqwuqq] (3.9400000937441466,4.996400011249289) -- (3.9550000933061065,4.997975008397541);
\draw[line width=2pt,color=qqwuqq] (3.9550000933061065,4.997975008397541) -- (3.9700000928680663,4.999100005572076);
\draw[line width=2pt,color=qqwuqq] (3.9700000928680663,4.999100005572076) -- (3.985000092430026,4.999775002772893);
\draw[line width=2pt,color=qqwuqq] (3.985000092430026,4.999775002772893) -- (4.0000000919919865,4.999999999999991);
\draw[line width=2pt,color=qqwuqq] (4.0000000919919865,4.999999999999991) -- (4.015000091553946,4.999774997253374);
\draw[line width=2pt,color=qqwuqq] (4.015000091553946,4.999774997253374) -- (4.030000091115906,4.999099994533037);
\draw[line width=2pt,color=qqwuqq] (4.030000091115906,4.999099994533037) -- (4.045000090677866,4.9979749918389835);
\draw[line width=2pt,color=qqwuqq] (4.045000090677866,4.9979749918389835) -- (4.060000090239826,4.996399989171213);
\draw[line width=2pt,color=qqwuqq] (4.060000090239826,4.996399989171213) -- (4.075000089801786,4.994374986529724);
\draw[line width=2pt,color=qqwuqq] (4.075000089801786,4.994374986529724) -- (4.090000089363746,4.991899983914518);
\draw[line width=2pt,color=qqwuqq] (4.090000089363746,4.991899983914518) -- (4.1050000889257054,4.988974981325594);
\draw[line width=2pt,color=qqwuqq] (4.1050000889257054,4.988974981325594) -- (4.120000088487665,4.985599978762952);
\draw[line width=2pt,color=qqwuqq] (4.120000088487665,4.985599978762952) -- (4.135000088049625,4.9817749762265935);
\draw[line width=2pt,color=qqwuqq] (4.135000088049625,4.9817749762265935) -- (4.150000087611585,4.977499973716517);
\draw[line width=2pt,color=qqwuqq] (4.150000087611585,4.977499973716517) -- (4.165000087173545,4.972774971232723);
\draw[line width=2pt,color=qqwuqq] (4.165000087173545,4.972774971232723) -- (4.180000086735505,4.967599968775211);
\draw[line width=2pt,color=qqwuqq] (4.180000086735505,4.967599968775211) -- (4.195000086297465,4.961974966343981);
\draw[line width=2pt,color=qqwuqq] (4.195000086297465,4.961974966343981) -- (4.210000085859424,4.955899963939034);
\draw[line width=2pt,color=qqwuqq] (4.210000085859424,4.955899963939034) -- (4.225000085421384,4.94937496156037);
\draw[line width=2pt,color=qqwuqq] (4.225000085421384,4.94937496156037) -- (4.240000084983344,4.942399959207988);
\draw[line width=2pt,color=qqwuqq] (4.240000084983344,4.942399959207988) -- (4.255000084545304,4.934974956881888);
\draw[line width=2pt,color=qqwuqq] (4.255000084545304,4.934974956881888) -- (4.270000084107264,4.92709995458207);
\draw[line width=2pt,color=qqwuqq] (4.270000084107264,4.92709995458207) -- (4.285000083669224,4.918774952308535);
\draw[line width=2pt,color=qqwuqq] (4.285000083669224,4.918774952308535) -- (4.3000000832311835,4.909999950061283);
\draw[line width=2pt,color=qqwuqq] (4.3000000832311835,4.909999950061283) -- (4.315000082793143,4.900774947840313);
\draw[line width=2pt,color=qqwuqq] (4.315000082793143,4.900774947840313) -- (4.330000082355103,4.8910999456456254);
\draw[line width=2pt,color=qqwuqq] (4.330000082355103,4.8910999456456254) -- (4.345000081917063,4.88097494347722);
\draw[line width=2pt,color=qqwuqq] (4.345000081917063,4.88097494347722) -- (4.360000081479023,4.870399941335097);
\draw[line width=2pt,color=qqwuqq] (4.360000081479023,4.870399941335097) -- (4.375000081040983,4.859374939219256);
\draw[line width=2pt,color=qqwuqq] (4.375000081040983,4.859374939219256) -- (4.390000080602943,4.847899937129698);
\draw[line width=2pt,color=qqwuqq] (4.390000080602943,4.847899937129698) -- (4.4050000801649025,4.835974935066423);
\draw[line width=2pt,color=qqwuqq] (4.4050000801649025,4.835974935066423) -- (4.420000079726862,4.8235999330294295);
\draw[line width=2pt,color=qqwuqq] (4.420000079726862,4.8235999330294295) -- (4.435000079288822,4.8107749310187184);
\draw[line width=2pt,color=qqwuqq] (4.435000079288822,4.8107749310187184) -- (4.450000078850782,4.79749992903429);
\draw[line width=2pt,color=qqwuqq] (4.450000078850782,4.79749992903429) -- (4.465000078412742,4.783774927076144);
\draw[line width=2pt,color=qqwuqq] (4.465000078412742,4.783774927076144) -- (4.480000077974702,4.76959992514428);
\draw[line width=2pt,color=qqwuqq] (4.480000077974702,4.76959992514428) -- (4.495000077536662,4.754974923238699);
\draw[line width=2pt,color=qqwuqq] (4.495000077536662,4.754974923238699) -- (4.5100000770986215,4.7398999213594);
\draw[line width=2pt,color=qqwuqq] (4.5100000770986215,4.7398999213594) -- (4.525000076660581,4.724374919506384);
\draw[line width=2pt,color=qqwuqq] (4.525000076660581,4.724374919506384) -- (4.540000076222541,4.70839991767965);
\draw[line width=2pt,color=qqwuqq] (4.540000076222541,4.70839991767965) -- (4.555000075784501,4.691974915879198);
\draw[line width=2pt,color=qqwuqq] (4.555000075784501,4.691974915879198) -- (4.570000075346461,4.675099914105029);
\draw[line width=2pt,color=qqwuqq] (4.570000075346461,4.675099914105029) -- (4.585000074908421,4.657774912357142);
\draw[line width=2pt,color=qqwuqq] (4.585000074908421,4.657774912357142) -- (4.600000074470381,4.639999910635538);
\draw[line width=2pt,color=qqwuqq] (4.600000074470381,4.639999910635538) -- (4.6150000740323405,4.621774908940216);
\draw[line width=2pt,color=qqwuqq] (4.6150000740323405,4.621774908940216) -- (4.6300000735943,4.603099907271176);
\draw[line width=2pt,color=qqwuqq] (4.6300000735943,4.603099907271176) -- (4.64500007315626,4.583974905628419);
\draw[line width=2pt,color=qqwuqq] (4.64500007315626,4.583974905628419) -- (4.66000007271822,4.564399904011944);
\draw[line width=2pt,color=qqwuqq] (4.66000007271822,4.564399904011944) -- (4.67500007228018,4.544374902421752);
\draw[line width=2pt,color=qqwuqq] (4.67500007228018,4.544374902421752) -- (4.69000007184214,4.523899900857842);
\draw[line width=2pt,color=qqwuqq] (4.69000007184214,4.523899900857842) -- (4.7050000714041,4.502974899320215);
\draw[line width=2pt,color=qqwuqq] (4.7050000714041,4.502974899320215) -- (4.7200000709660594,4.481599897808869);
\draw[line width=2pt,color=qqwuqq] (4.7200000709660594,4.481599897808869) -- (4.735000070528019,4.459774896323807);
\draw[line width=2pt,color=qqwuqq] (4.735000070528019,4.459774896323807) -- (4.750000070089979,4.437499894865026);
\draw[line width=2pt,color=qqwuqq] (4.750000070089979,4.437499894865026) -- (4.765000069651939,4.414774893432528);
\draw[line width=2pt,color=qqwuqq] (4.765000069651939,4.414774893432528) -- (4.780000069213899,4.391599892026313);
\draw[line width=2pt,color=qqwuqq] (4.780000069213899,4.391599892026313) -- (4.795000068775859,4.36797489064638);
\draw[line width=2pt,color=qqwuqq] (4.795000068775859,4.36797489064638) -- (4.810000068337819,4.34389988929273);
\draw[line width=2pt,color=qqwuqq] (4.810000068337819,4.34389988929273) -- (4.825000067899778,4.319374887965361);
\draw[line width=2pt,color=qqwuqq] (4.825000067899778,4.319374887965361) -- (4.840000067461738,4.294399886664275);
\draw[line width=2pt,color=qqwuqq] (4.840000067461738,4.294399886664275) -- (4.855000067023698,4.268974885389472);
\draw[line width=2pt,color=qqwuqq] (4.855000067023698,4.268974885389472) -- (4.870000066585658,4.243099884140951);
\draw[line width=2pt,color=qqwuqq] (4.870000066585658,4.243099884140951) -- (4.885000066147618,4.216774882918712);
\draw[line width=2pt,color=qqwuqq] (4.885000066147618,4.216774882918712) -- (4.900000065709578,4.1899998817227555);
\draw[line width=2pt,color=qqwuqq] (4.900000065709578,4.1899998817227555) -- (4.9150000652715375,4.162774880553082);
\draw[line width=2pt,color=qqwuqq] (4.9150000652715375,4.162774880553082) -- (4.930000064833497,4.13509987940969);
\draw[line width=2pt,color=qqwuqq] (4.930000064833497,4.13509987940969) -- (4.945000064395457,4.106974878292582);
\draw[line width=2pt,color=qqwuqq] (4.945000064395457,4.106974878292582) -- (4.960000063957417,4.078399877201755);
\draw[line width=2pt,color=qqwuqq] (4.960000063957417,4.078399877201755) -- (4.975000063519377,4.049374876137211);
\draw[line width=2pt,color=qqwuqq] (4.975000063519377,4.049374876137211) -- (4.990000063081337,4.019899875098949);
\draw[line width=2pt,color=qqwuqq] (4.990000063081337,4.019899875098949) -- (5.005000062643297,3.98997487408697);
\draw[line width=2pt,color=qqwuqq] (5.005000062643297,3.98997487408697) -- (5.0200000622052565,3.959599873101273);
\draw[line width=2pt,color=qqwuqq] (5.0200000622052565,3.959599873101273) -- (5.035000061767216,3.928774872141858);
\draw[line width=2pt,color=qqwuqq] (5.035000061767216,3.928774872141858) -- (5.050000061329176,3.8974998712087263);
\draw[line width=2pt,color=qqwuqq] (5.050000061329176,3.8974998712087263) -- (5.065000060891136,3.8657748703018764);
\draw[line width=2pt,color=qqwuqq] (5.065000060891136,3.8657748703018764) -- (5.080000060453096,3.833599869421309);
\draw[line width=2pt,color=qqwuqq] (5.080000060453096,3.833599869421309) -- (5.095000060015056,3.800974868567024);
\draw[line width=2pt,color=qqwuqq] (5.095000060015056,3.800974868567024) -- (5.110000059577016,3.767899867739022);
\draw[line width=2pt,color=qqwuqq] (5.110000059577016,3.767899867739022) -- (5.1250000591389755,3.7343748669373014);
\draw[line width=2pt,color=qqwuqq] (5.1250000591389755,3.7343748669373014) -- (5.140000058700935,3.700399866161864);
\draw[line width=2pt,color=qqwuqq] (5.140000058700935,3.700399866161864) -- (5.155000058262895,3.665974865412709);
\draw[line width=2pt,color=qqwuqq] (5.155000058262895,3.665974865412709) -- (5.170000057824855,3.631099864689836);
\draw[line width=2pt,color=qqwuqq] (5.170000057824855,3.631099864689836) -- (5.185000057386815,3.5957748639932454);
\draw[line width=2pt,color=qqwuqq] (5.185000057386815,3.5957748639932454) -- (5.200000056948775,3.559999863322937);
\draw[line width=2pt,color=qqwuqq] (5.200000056948775,3.559999863322937) -- (5.215000056510735,3.523774862678912);
\draw[line width=2pt,color=qqwuqq] (5.215000056510735,3.523774862678912) -- (5.2300000560726945,3.4870998620611684);
\draw[line width=2pt,color=qqwuqq] (5.2300000560726945,3.4870998620611684) -- (5.245000055634654,3.4499748614697077);
\draw[line width=2pt,color=qqwuqq] (5.245000055634654,3.4499748614697077) -- (5.260000055196614,3.412399860904529);
\draw[line width=2pt,color=qqwuqq] (5.260000055196614,3.412399860904529) -- (5.275000054758574,3.3743748603656334);
\draw[line width=2pt,color=qqwuqq] (5.275000054758574,3.3743748603656334) -- (5.290000054320534,3.3358998598530194);
\draw[line width=2pt,color=qqwuqq] (5.290000054320534,3.3358998598530194) -- (5.305000053882494,3.2969748593666885);
\draw[line width=2pt,color=qqwuqq] (5.305000053882494,3.2969748593666885) -- (5.320000053444454,3.25759985890664);
\draw[line width=2pt,color=qqwuqq] (5.320000053444454,3.25759985890664) -- (5.3350000530064134,3.2177748584728736);
\draw[line width=2pt,color=qqwuqq] (5.3350000530064134,3.2177748584728736) -- (5.350000052568373,3.1774998580653895);
\draw[line width=2pt,color=qqwuqq] (5.350000052568373,3.1774998580653895) -- (5.365000052130333,3.1367748576841876);
\draw[line width=2pt,color=qqwuqq] (5.365000052130333,3.1367748576841876) -- (5.380000051692293,3.095599857329269);
\draw[line width=2pt,color=qqwuqq] (5.380000051692293,3.095599857329269) -- (5.395000051254253,3.053974857000632);
\draw[line width=2pt,color=qqwuqq] (5.395000051254253,3.053974857000632) -- (5.410000050816213,3.0118998566982773);
\draw[line width=2pt,color=qqwuqq] (5.410000050816213,3.0118998566982773) -- (5.425000050378173,2.9693748564222058);
\draw[line width=2pt,color=qqwuqq] (5.425000050378173,2.9693748564222058) -- (5.440000049940132,2.926399856172416);
\draw[line width=2pt,color=qqwuqq] (5.440000049940132,2.926399856172416) -- (5.455000049502092,2.882974855948909);
\draw[line width=2pt,color=qqwuqq] (5.455000049502092,2.882974855948909) -- (5.470000049064052,2.839099855751684);
\draw[line width=2pt,color=qqwuqq] (5.470000049064052,2.839099855751684) -- (5.485000048626012,2.794774855580742);
\draw[line width=2pt,color=qqwuqq] (5.485000048626012,2.794774855580742) -- (5.500000048187972,2.7499998554360823);
\draw[line width=2pt,color=qqwuqq] (5.500000048187972,2.7499998554360823) -- (5.515000047749932,2.7047748553177047);
\draw[line width=2pt,color=qqwuqq] (5.515000047749932,2.7047748553177047) -- (5.5300000473118915,2.6590998552256098);
\draw[line width=2pt,color=qqwuqq] (5.5300000473118915,2.6590998552256098) -- (5.545000046873851,2.612974855159797);
\draw[line width=2pt,color=qqwuqq] (5.545000046873851,2.612974855159797) -- (5.560000046435811,2.5663998551202667);
\draw[line width=2pt,color=qqwuqq] (5.560000046435811,2.5663998551202667) -- (5.575000045997771,2.519374855107019);
\draw[line width=2pt,color=qqwuqq] (5.575000045997771,2.519374855107019) -- (5.590000045559731,2.4718998551200535);
\draw[line width=2pt,color=qqwuqq] (5.590000045559731,2.4718998551200535) -- (5.605000045121691,2.4239748551593703);
\draw[line width=2pt,color=qqwuqq] (5.605000045121691,2.4239748551593703) -- (5.620000044683651,2.3755998552249697);
\draw[line width=2pt,color=qqwuqq] (5.620000044683651,2.3755998552249697) -- (5.6350000442456105,2.3267748553168515);
\draw[line width=2pt,color=qqwuqq] (5.6350000442456105,2.3267748553168515) -- (5.65000004380757,2.277499855435016);
\draw[line width=2pt,color=qqwuqq] (5.65000004380757,2.277499855435016) -- (5.66500004336953,2.2277748555794625);
\draw[line width=2pt,color=qqwuqq] (5.66500004336953,2.2277748555794625) -- (5.68000004293149,2.1775998557501914);
\draw[line width=2pt,color=qqwuqq] (5.68000004293149,2.1775998557501914) -- (5.69500004249345,2.126974855947203);
\draw[line width=2pt,color=qqwuqq] (5.69500004249345,2.126974855947203) -- (5.71000004205541,2.075899856170497);
\draw[line width=2pt,color=qqwuqq] (5.71000004205541,2.075899856170497) -- (5.72500004161737,2.024374856420073);
\draw[line width=2pt,color=qqwuqq] (5.72500004161737,2.024374856420073) -- (5.7400000411793295,1.9723998566959318);
\draw[line width=2pt,color=qqwuqq] (5.7400000411793295,1.9723998566959318) -- (5.755000040741289,1.9199748569980728);
\draw[line width=2pt,color=qqwuqq] (5.755000040741289,1.9199748569980728) -- (5.770000040303249,1.867099857326496);
\draw[line width=2pt,color=qqwuqq] (5.770000040303249,1.867099857326496) -- (5.785000039865209,1.813774857681202);
\draw[line width=2pt,color=qqwuqq] (5.785000039865209,1.813774857681202) -- (5.800000039427169,1.7599998580621903);
\draw[line width=2pt,color=qqwuqq] (5.800000039427169,1.7599998580621903) -- (5.815000038989129,1.7057748584694612);
\draw[line width=2pt,color=qqwuqq] (5.815000038989129,1.7057748584694612) -- (5.830000038551089,1.6510998589030144);
\draw[line width=2pt,color=qqwuqq] (5.830000038551089,1.6510998589030144) -- (5.8450000381130485,1.5959748593628498);
\draw[line width=2pt,color=qqwuqq] (5.8450000381130485,1.5959748593628498) -- (5.860000037675008,1.5403998598489674);
\draw[line width=2pt,color=qqwuqq] (5.860000037675008,1.5403998598489674) -- (5.875000037236968,1.4843748603613678);
\draw[line width=2pt,color=qqwuqq] (5.875000037236968,1.4843748603613678) -- (5.890000036798928,1.4278998609000508);
\draw[line width=2pt,color=qqwuqq] (5.890000036798928,1.4278998609000508) -- (5.905000036360888,1.3709748614650157);
\draw[line width=2pt,color=qqwuqq] (5.905000036360888,1.3709748614650157) -- (5.920000035922848,1.3135998620562632);
\draw[line width=2pt,color=qqwuqq] (5.920000035922848,1.3135998620562632) -- (5.935000035484808,1.2557748626737935);
\draw[line width=2pt,color=qqwuqq] (5.935000035484808,1.2557748626737935) -- (5.950000035046767,1.197499863317606);
\draw[line width=2pt,color=qqwuqq] (5.950000035046767,1.197499863317606) -- (5.965000034608727,1.1387748639877007);
\draw[line width=2pt,color=qqwuqq] (5.965000034608727,1.1387748639877007) -- (5.980000034170687,1.0795998646840776);
\draw[line width=2pt,color=qqwuqq] (5.980000034170687,1.0795998646840776) -- (5.995000033732647,1.0199748654067373);
\draw[line width=2pt,color=qqwuqq] (5.995000033732647,1.0199748654067373) -- (6.010000033294607,0.9598998661556797);
\draw[line width=2pt,color=qqwuqq] (6.010000033294607,0.9598998661556797) -- (6.025000032856567,0.8993748669309038);
\draw[line width=2pt,color=qqwuqq] (6.025000032856567,0.8993748669309038) -- (6.040000032418527,0.8383998677324103);
\draw[line width=2pt,color=qqwuqq] (6.040000032418527,0.8383998677324103) -- (6.055000031980486,0.7769748685601998);
\draw[line width=2pt,color=qqwuqq] (6.055000031980486,0.7769748685601998) -- (6.070000031542446,0.7150998694142716);
\draw[line width=2pt,color=qqwuqq] (6.070000031542446,0.7150998694142716) -- (6.085000031104406,0.6527748702946257);
\draw[line width=2pt,color=qqwuqq] (6.085000031104406,0.6527748702946257) -- (6.100000030666366,0.589999871201262);
\draw[line width=2pt,color=qqwuqq] (6.100000030666366,0.589999871201262) -- (6.115000030228326,0.5267748721341805);
\draw[line width=2pt,color=qqwuqq] (6.115000030228326,0.5267748721341805) -- (6.130000029790286,0.4630998730933822);
\draw[line width=2pt,color=qqwuqq] (6.130000029790286,0.4630998730933822) -- (6.1450000293522455,0.3989748740788661);
\draw[line width=2pt,color=qqwuqq] (6.1450000293522455,0.3989748740788661) -- (6.160000028914205,0.3343998750906323);
\draw[line width=2pt,color=qqwuqq] (6.160000028914205,0.3343998750906323) -- (6.175000028476165,0.2693748761286807);
\draw[line width=2pt,color=qqwuqq] (6.175000028476165,0.2693748761286807) -- (6.190000028038125,0.2038998771930114);
\draw[line width=2pt,color=qqwuqq] (6.190000028038125,0.2038998771930114) -- (6.205000027600085,0.1379748782836243);
\draw[line width=2pt,color=qqwuqq] (6.205000027600085,0.1379748782836243) -- (6.220000027162045,0.07159987940052037);
\draw[line width=2pt,color=qqwuqq] (6.220000027162045,0.07159987940052037) -- (6.235000026724005,0.0047748805436986785);
\draw[line width=2pt,color=qqwuqq] (6.235000026724005,0.0047748805436986785) -- (6.2500000262859645,-0.06250011828684077);
\draw[line width=2pt,color=qqwuqq] (6.2500000262859645,-0.06250011828684077) -- (6.265000025847924,-0.13022511709109796);
\draw[line width=2pt,color=qqwuqq] (6.265000025847924,-0.13022511709109796) -- (6.280000025409884,-0.1984001158690729);
\draw[line width=2pt,color=qqwuqq] (6.280000025409884,-0.1984001158690729) -- (6.295000024971844,-0.2670251146207647);
\draw[line width=2pt,color=qqwuqq] (6.295000024971844,-0.2670251146207647) -- (6.310000024533804,-0.3361001133461752);
\draw[line width=2pt,color=qqwuqq] (6.310000024533804,-0.3361001133461752) -- (6.325000024095764,-0.4056251120453025);
\draw[line width=2pt,color=qqwuqq] (6.325000024095764,-0.4056251120453025) -- (6.340000023657724,-0.47560011071814756);
\draw[line width=2pt,color=qqwuqq] (6.340000023657724,-0.47560011071814756) -- (6.3550000232196835,-0.5460251093647095);
\draw[line width=2pt,color=qqwuqq] (6.3550000232196835,-0.5460251093647095) -- (6.370000022781643,-0.6169001079849901);
\draw[line width=2pt,color=qqwuqq] (6.370000022781643,-0.6169001079849901) -- (6.385000022343603,-0.6882251065789875);
\draw[line width=2pt,color=qqwuqq] (6.385000022343603,-0.6882251065789875) -- (6.400000021905563,-0.7600001051467036);
\draw[line width=2pt,color=qqwuqq] (6.400000021905563,-0.7600001051467036) -- (6.415000021467523,-0.8322251036881365);
\draw[line width=2pt,color=qqwuqq] (6.415000021467523,-0.8322251036881365) -- (6.430000021029483,-0.9049001022032863);
\draw[line width=2pt,color=qqwuqq] (6.430000021029483,-0.9049001022032863) -- (6.445000020591443,-0.9780251006921548);
\draw[line width=2pt,color=qqwuqq] (6.445000020591443,-0.9780251006921548) -- (6.4600000201534025,-1.051600099154741);
\draw[line width=2pt,color=qqwuqq] (6.4600000201534025,-1.051600099154741) -- (6.475000019715362,-1.125625097591044);
\draw[line width=2pt,color=qqwuqq] (6.475000019715362,-1.125625097591044) -- (6.490000019277322,-1.2001000960010648);
\draw[line width=2pt,color=qqwuqq] (6.490000019277322,-1.2001000960010648) -- (6.505000018839282,-1.2750250943848034);
\draw[line width=2pt,color=qqwuqq] (6.505000018839282,-1.2750250943848034) -- (6.520000018401242,-1.3504000927422597);
\draw[line width=2pt,color=qqwuqq] (6.520000018401242,-1.3504000927422597) -- (6.535000017963202,-1.4262250910734329);
\draw[line width=2pt,color=qqwuqq] (6.535000017963202,-1.4262250910734329) -- (6.550000017525162,-1.5025000893783247);
\draw[line width=2pt,color=qqwuqq] (6.550000017525162,-1.5025000893783247) -- (6.565000017087121,-1.5792250876569334);
\draw[line width=2pt,color=qqwuqq] (6.565000017087121,-1.5792250876569334) -- (6.580000016649081,-1.6564000859092598);
\draw[line width=2pt,color=qqwuqq] (6.580000016649081,-1.6564000859092598) -- (6.595000016211041,-1.734025084135304);
\draw[line width=2pt,color=qqwuqq] (6.595000016211041,-1.734025084135304) -- (6.610000015773001,-1.812100082335066);
\draw[line width=2pt,color=qqwuqq] (6.610000015773001,-1.812100082335066) -- (6.625000015334961,-1.8906250805085447);
\draw[line width=2pt,color=qqwuqq] (6.625000015334961,-1.8906250805085447) -- (6.640000014896921,-1.9696000786557413);
\draw[line width=2pt,color=qqwuqq] (6.640000014896921,-1.9696000786557413) -- (6.655000014458881,-2.0490250767766556);
\draw[line width=2pt,color=qqwuqq] (6.655000014458881,-2.0490250767766556) -- (6.67000001402084,-2.1289000748712876);
\draw[line width=2pt,color=qqwuqq] (6.67000001402084,-2.1289000748712876) -- (6.6850000135828,-2.2092250729396374);
\draw[line width=2pt,color=qqwuqq] (6.6850000135828,-2.2092250729396374) -- (6.70000001314476,-2.290000070981705);
\draw[line width=2pt,color=qqwuqq] (6.70000001314476,-2.290000070981705) -- (6.71500001270672,-2.3712250689974894);
\draw[line width=2pt,color=qqwuqq] (6.71500001270672,-2.3712250689974894) -- (6.73000001226868,-2.4529000669869916);
\draw[line width=2pt,color=qqwuqq] (6.73000001226868,-2.4529000669869916) -- (6.74500001183064,-2.5350250649502124);
\draw[line width=2pt,color=qqwuqq] (6.74500001183064,-2.5350250649502124) -- (6.7600000113925995,-2.617600062887149);
\draw[line width=2pt,color=qqwuqq] (6.7600000113925995,-2.617600062887149) -- (6.775000010954559,-2.7006250607978046);
\draw[line width=2pt,color=qqwuqq] (6.775000010954559,-2.7006250607978046) -- (6.790000010516519,-2.784100058682178);
\draw[line width=2pt,color=qqwuqq] (6.790000010516519,-2.784100058682178) -- (6.805000010078479,-2.868025056540268);
\draw[line width=2pt,color=qqwuqq] (6.805000010078479,-2.868025056540268) -- (6.820000009640439,-2.9524000543720756);
\draw[line width=2pt,color=qqwuqq] (6.820000009640439,-2.9524000543720756) -- (6.835000009202399,-3.037225052177602);
\draw[line width=2pt,color=qqwuqq] (6.835000009202399,-3.037225052177602) -- (6.850000008764359,-3.1225000499568445);
\draw[line width=2pt,color=qqwuqq] (6.850000008764359,-3.1225000499568445) -- (6.8650000083263185,-3.2082250477098047);
\draw[line width=2pt,color=qqwuqq] (6.8650000083263185,-3.2082250477098047) -- (6.880000007888278,-3.2944000454364843);
\draw[line width=2pt,color=qqwuqq] (6.880000007888278,-3.2944000454364843) -- (6.895000007450238,-3.38102504313688);
\draw[line width=2pt,color=qqwuqq] (6.895000007450238,-3.38102504313688) -- (6.910000007012198,-3.4681000408109934);
\draw[line width=2pt,color=qqwuqq] (6.910000007012198,-3.4681000408109934) -- (6.925000006574158,-3.5556250384588246);
\draw[line width=2pt,color=qqwuqq] (6.925000006574158,-3.5556250384588246) -- (6.940000006136118,-3.6436000360803718);
\draw[line width=2pt,color=qqwuqq] (6.940000006136118,-3.6436000360803718) -- (6.955000005698078,-3.7320250336756384);
\draw[line width=2pt,color=qqwuqq] (6.955000005698078,-3.7320250336756384) -- (6.9700000052600375,-3.820900031244623);
\draw[line width=2pt,color=qqwuqq] (6.9700000052600375,-3.820900031244623) -- (6.985000004821997,-3.910225028787325);
\draw[line width=2pt,color=qqwuqq] (6.985000004821997,-3.910225028787325) -- (7.000000004383957,-4.000000026303743);
\draw[line width=2pt,color=ccqqqq] (1.0000001796000004,5.999998922400028) -- (1.0000001796000004,5.999998922400028);
\draw[line width=2pt,color=ccqqqq] (1.0000001796000004,5.999998922400028) -- (1.0150001791619605,5.910223930403127);
\draw[line width=2pt,color=ccqqqq] (1.0150001791619605,5.910223930403127) -- (1.0300001787239206,5.820898938379946);
\draw[line width=2pt,color=ccqqqq] (1.0300001787239206,5.820898938379946) -- (1.0450001782858807,5.732023946330477);
\draw[line width=2pt,color=ccqqqq] (1.0450001782858807,5.732023946330477) -- (1.0600001778478407,5.643598954254728);
\draw[line width=2pt,color=ccqqqq] (1.0600001778478407,5.643598954254728) -- (1.0750001774098008,5.555623962152696);
\draw[line width=2pt,color=ccqqqq] (1.0750001774098008,5.555623962152696) -- (1.0900001769717609,5.468098970024384);
\draw[line width=2pt,color=ccqqqq] (1.0900001769717609,5.468098970024384) -- (1.105000176533721,5.381023977869788);
\draw[line width=2pt,color=ccqqqq] (1.105000176533721,5.381023977869788) -- (1.120000176095681,5.2943989856889075);
\draw[line width=2pt,color=ccqqqq] (1.120000176095681,5.2943989856889075) -- (1.135000175657641,5.208223993481747);
\draw[line width=2pt,color=ccqqqq] (1.135000175657641,5.208223993481747) -- (1.1500001752196012,5.122499001248306);
\draw[line width=2pt,color=ccqqqq] (1.1500001752196012,5.122499001248306) -- (1.1650001747815613,5.037224008988579);
\draw[line width=2pt,color=ccqqqq] (1.1650001747815613,5.037224008988579) -- (1.1800001743435213,4.952399016702569);
\draw[line width=2pt,color=ccqqqq] (1.1800001743435213,4.952399016702569) -- (1.1950001739054814,4.868024024390279);
\draw[line width=2pt,color=ccqqqq] (1.1950001739054814,4.868024024390279) -- (1.2100001734674415,4.784099032051708);
\draw[line width=2pt,color=ccqqqq] (1.2100001734674415,4.784099032051708) -- (1.2250001730294016,4.7006240396868515);
\draw[line width=2pt,color=ccqqqq] (1.2250001730294016,4.7006240396868515) -- (1.2400001725913616,4.6175990472957125);
\draw[line width=2pt,color=ccqqqq] (1.2400001725913616,4.6175990472957125) -- (1.2550001721533217,4.535024054878294);
\draw[line width=2pt,color=ccqqqq] (1.2550001721533217,4.535024054878294) -- (1.2700001717152818,4.452899062434592);
\draw[line width=2pt,color=ccqqqq] (1.2700001717152818,4.452899062434592) -- (1.2850001712772419,4.371224069964606);
\draw[line width=2pt,color=ccqqqq] (1.2850001712772419,4.371224069964606) -- (1.300000170839202,4.289999077468337);
\draw[line width=2pt,color=ccqqqq] (1.300000170839202,4.289999077468337) -- (1.315000170401162,4.209224084945789);
\draw[line width=2pt,color=ccqqqq] (1.315000170401162,4.209224084945789) -- (1.330000169963122,4.128899092396958);
\draw[line width=2pt,color=ccqqqq] (1.330000169963122,4.128899092396958) -- (1.3450001695250822,4.0490240998218425);
\draw[line width=2pt,color=ccqqqq] (1.3450001695250822,4.0490240998218425) -- (1.3600001690870422,3.9695991072204446);
\draw[line width=2pt,color=ccqqqq] (1.3600001690870422,3.9695991072204446) -- (1.3750001686490023,3.8906241145927662);
\draw[line width=2pt,color=ccqqqq] (1.3750001686490023,3.8906241145927662) -- (1.3900001682109624,3.8120991219388056);
\draw[line width=2pt,color=ccqqqq] (1.3900001682109624,3.8120991219388056) -- (1.4050001677729225,3.73402412925856);
\draw[line width=2pt,color=ccqqqq] (1.4050001677729225,3.73402412925856) -- (1.4200001673348825,3.656399136552033);
\draw[line width=2pt,color=ccqqqq] (1.4200001673348825,3.656399136552033) -- (1.4350001668968426,3.579224143819225);
\draw[line width=2pt,color=ccqqqq] (1.4350001668968426,3.579224143819225) -- (1.4500001664588027,3.5024991510601353);
\draw[line width=2pt,color=ccqqqq] (1.4500001664588027,3.5024991510601353) -- (1.4650001660207628,3.42622415827476);
\draw[line width=2pt,color=ccqqqq] (1.4650001660207628,3.42622415827476) -- (1.4800001655827228,3.350399165463103);
\draw[line width=2pt,color=ccqqqq] (1.4800001655827228,3.350399165463103) -- (1.495000165144683,3.275024172625166);
\draw[line width=2pt,color=ccqqqq] (1.495000165144683,3.275024172625166) -- (1.510000164706643,3.2000991797609464);
\draw[line width=2pt,color=ccqqqq] (1.510000164706643,3.2000991797609464) -- (1.525000164268603,3.125624186870442);
\draw[line width=2pt,color=ccqqqq] (1.525000164268603,3.125624186870442) -- (1.5400001638305632,3.0515991939536553);
\draw[line width=2pt,color=ccqqqq] (1.5400001638305632,3.0515991939536553) -- (1.5550001633925232,2.978024201010588);
\draw[line width=2pt,color=ccqqqq] (1.5550001633925232,2.978024201010588) -- (1.5700001629544833,2.904899208041239);
\draw[line width=2pt,color=ccqqqq] (1.5700001629544833,2.904899208041239) -- (1.5850001625164434,2.8322242150456045);
\draw[line width=2pt,color=ccqqqq] (1.5850001625164434,2.8322242150456045) -- (1.6000001620784035,2.759999222023689);
\draw[line width=2pt,color=ccqqqq] (1.6000001620784035,2.759999222023689) -- (1.6150001616403635,2.688224228975492);
\draw[line width=2pt,color=ccqqqq] (1.6150001616403635,2.688224228975492) -- (1.6300001612023236,2.6168992359010135);
\draw[line width=2pt,color=ccqqqq] (1.6300001612023236,2.6168992359010135) -- (1.6450001607642837,2.5460242428002493);
\draw[line width=2pt,color=ccqqqq] (1.6450001607642837,2.5460242428002493) -- (1.6600001603262438,2.4755992496732038);
\draw[line width=2pt,color=ccqqqq] (1.6600001603262438,2.4755992496732038) -- (1.6750001598882038,2.4056242565198778);
\draw[line width=2pt,color=ccqqqq] (1.6750001598882038,2.4056242565198778) -- (1.690000159450164,2.3360992633402695);
\draw[line width=2pt,color=ccqqqq] (1.690000159450164,2.3360992633402695) -- (1.705000159012124,2.2670242701343764);
\draw[line width=2pt,color=ccqqqq] (1.705000159012124,2.2670242701343764) -- (1.720000158574084,2.198399276902201);
\draw[line width=2pt,color=ccqqqq] (1.720000158574084,2.198399276902201) -- (1.7350001581360441,2.130224283643745);
\draw[line width=2pt,color=ccqqqq] (1.7350001581360441,2.130224283643745) -- (1.7500001576980042,2.062499290359007);
\draw[line width=2pt,color=ccqqqq] (1.7500001576980042,2.062499290359007) -- (1.7650001572599643,1.9952242970479848);
\draw[line width=2pt,color=ccqqqq] (1.7650001572599643,1.9952242970479848) -- (1.7800001568219244,1.9283993037106795);
\draw[line width=2pt,color=ccqqqq] (1.7800001568219244,1.9283993037106795) -- (1.7950001563838844,1.8620243103470937);
\draw[line width=2pt,color=ccqqqq] (1.7950001563838844,1.8620243103470937) -- (1.8100001559458445,1.7960993169572266);
\draw[line width=2pt,color=ccqqqq] (1.8100001559458445,1.7960993169572266) -- (1.8250001555078046,1.7306243235410745);
\draw[line width=2pt,color=ccqqqq] (1.8250001555078046,1.7306243235410745) -- (1.8400001550697647,1.6655993300986394);
\draw[line width=2pt,color=ccqqqq] (1.8400001550697647,1.6655993300986394) -- (1.8550001546317247,1.6010243366299246);
\draw[line width=2pt,color=ccqqqq] (1.8550001546317247,1.6010243366299246) -- (1.8700001541936848,1.5368993431349276);
\draw[line width=2pt,color=ccqqqq] (1.8700001541936848,1.5368993431349276) -- (1.885000153755645,1.4732243496136457);
\draw[line width=2pt,color=ccqqqq] (1.885000153755645,1.4732243496136457) -- (1.900000153317605,1.4099993560660815);
\draw[line width=2pt,color=ccqqqq] (1.900000153317605,1.4099993560660815) -- (1.915000152879565,1.3472243624922369);
\draw[line width=2pt,color=ccqqqq] (1.915000152879565,1.3472243624922369) -- (1.9300001524415251,1.28489936889211);
\draw[line width=2pt,color=ccqqqq] (1.9300001524415251,1.28489936889211) -- (1.9450001520034852,1.223024375265699);
\draw[line width=2pt,color=ccqqqq] (1.9450001520034852,1.223024375265699) -- (1.9600001515654453,1.161599381613005);
\draw[line width=2pt,color=ccqqqq] (1.9600001515654453,1.161599381613005) -- (1.9750001511274053,1.1006243879340314);
\draw[line width=2pt,color=ccqqqq] (1.9750001511274053,1.1006243879340314) -- (1.9900001506893654,1.0400993942287746);
\draw[line width=2pt,color=ccqqqq] (1.9900001506893654,1.0400993942287746) -- (2.0050001502513255,0.9800244004972338);
\draw[line width=2pt,color=ccqqqq] (2.0050001502513255,0.9800244004972338) -- (2.0200001498132854,0.9203994067394126);
\draw[line width=2pt,color=ccqqqq] (2.0200001498132854,0.9203994067394126) -- (2.035000149375245,0.8612244129553086);
\draw[line width=2pt,color=ccqqqq] (2.035000149375245,0.8612244129553086) -- (2.050000148937205,0.8024994191449224);
\draw[line width=2pt,color=ccqqqq] (2.050000148937205,0.8024994191449224) -- (2.065000148499165,0.744224425308254);
\draw[line width=2pt,color=ccqqqq] (2.065000148499165,0.744224425308254) -- (2.0800001480611248,0.6863994314453028);
\draw[line width=2pt,color=ccqqqq] (2.0800001480611248,0.6863994314453028) -- (2.0950001476230846,0.6290244375560694);
\draw[line width=2pt,color=ccqqqq] (2.0950001476230846,0.6290244375560694) -- (2.1100001471850445,0.5720994436405538);
\draw[line width=2pt,color=ccqqqq] (2.1100001471850445,0.5720994436405538) -- (2.1250001467470043,0.5156244496987554);
\draw[line width=2pt,color=ccqqqq] (2.1250001467470043,0.5156244496987554) -- (2.140000146308964,0.45959945573067484);
\draw[line width=2pt,color=ccqqqq] (2.140000146308964,0.45959945573067484) -- (2.155000145870924,0.40402446173631157);
\draw[line width=2pt,color=ccqqqq] (2.155000145870924,0.40402446173631157) -- (2.170000145432884,0.34889946771566605);
\draw[line width=2pt,color=ccqqqq] (2.170000145432884,0.34889946771566605) -- (2.1850001449948437,0.2942244736687383);
\draw[line width=2pt,color=ccqqqq] (2.1850001449948437,0.2942244736687383) -- (2.2000001445568036,0.2399994795955278);
\draw[line width=2pt,color=ccqqqq] (2.2000001445568036,0.2399994795955278) -- (2.2150001441187634,0.1862244854960351);
\draw[line width=2pt,color=ccqqqq] (2.2150001441187634,0.1862244854960351) -- (2.2300001436807233,0.13289949137026014);
\draw[line width=2pt,color=ccqqqq] (2.2300001436807233,0.13289949137026014) -- (2.245000143242683,0.08002449721820248);
\draw[line width=2pt,color=ccqqqq] (2.245000143242683,0.08002449721820248) -- (2.260000142804643,0.027599503039862583);
\draw[line width=2pt,color=ccqqqq] (2.260000142804643,0.027599503039862583) -- (2.275000142366603,-0.024375491164759566);
\draw[line width=2pt,color=ccqqqq] (2.275000142366603,-0.024375491164759566) -- (2.2900001419285627,-0.07590048539566441);
\draw[line width=2pt,color=ccqqqq] (2.2900001419285627,-0.07590048539566441) -- (2.3050001414905226,-0.1269754796528515);
\draw[line width=2pt,color=ccqqqq] (2.3050001414905226,-0.1269754796528515) -- (2.3200001410524824,-0.17760047393632084);
\draw[line width=2pt,color=ccqqqq] (2.3200001410524824,-0.17760047393632084) -- (2.3350001406144423,-0.22777546824607287);
\draw[line width=2pt,color=ccqqqq] (2.3350001406144423,-0.22777546824607287) -- (2.350000140176402,-0.2775004625821076);
\draw[line width=2pt,color=ccqqqq] (2.350000140176402,-0.2775004625821076) -- (2.365000139738362,-0.3267754569444241);
\draw[line width=2pt,color=ccqqqq] (2.365000139738362,-0.3267754569444241) -- (2.380000139300322,-0.37560045133302333);
\draw[line width=2pt,color=ccqqqq] (2.380000139300322,-0.37560045133302333) -- (2.3950001388622817,-0.4239754457479048);
\draw[line width=2pt,color=ccqqqq] (2.3950001388622817,-0.4239754457479048) -- (2.4100001384242415,-0.47190044018906896);
\draw[line width=2pt,color=ccqqqq] (2.4100001384242415,-0.47190044018906896) -- (2.4250001379862014,-0.5193754346565154);
\draw[line width=2pt,color=ccqqqq] (2.4250001379862014,-0.5193754346565154) -- (2.4400001375481613,-0.566400429150244);
\draw[line width=2pt,color=ccqqqq] (2.4400001375481613,-0.566400429150244) -- (2.455000137110121,-0.6129754236702554);
\draw[line width=2pt,color=ccqqqq] (2.455000137110121,-0.6129754236702554) -- (2.470000136672081,-0.659100418216549);
\draw[line width=2pt,color=ccqqqq] (2.470000136672081,-0.659100418216549) -- (2.485000136234041,-0.7047754127891253);
\draw[line width=2pt,color=ccqqqq] (2.485000136234041,-0.7047754127891253) -- (2.5000001357960007,-0.7500004073879833);
\draw[line width=2pt,color=ccqqqq] (2.5000001357960007,-0.7500004073879833) -- (2.5150001353579605,-0.7947754020131246);
\draw[line width=2pt,color=ccqqqq] (2.5150001353579605,-0.7947754020131246) -- (2.5300001349199204,-0.8391003966645476);
\draw[line width=2pt,color=ccqqqq] (2.5300001349199204,-0.8391003966645476) -- (2.5450001344818802,-0.8829753913422533);
\draw[line width=2pt,color=ccqqqq] (2.5450001344818802,-0.8829753913422533) -- (2.56000013404384,-0.9264003860462413);
\draw[line width=2pt,color=ccqqqq] (2.56000013404384,-0.9264003860462413) -- (2.5750001336058,-0.969375380776512);
\draw[line width=2pt,color=ccqqqq] (2.5750001336058,-0.969375380776512) -- (2.59000013316776,-1.0119003755330649);
\draw[line width=2pt,color=ccqqqq] (2.59000013316776,-1.0119003755330649) -- (2.6050001327297196,-1.0539753703159003);
\draw[line width=2pt,color=ccqqqq] (2.6050001327297196,-1.0539753703159003) -- (2.6200001322916795,-1.095600365125018);
\draw[line width=2pt,color=ccqqqq] (2.6200001322916795,-1.095600365125018) -- (2.6350001318536393,-1.136775359960418);
\draw[line width=2pt,color=ccqqqq] (2.6350001318536393,-1.136775359960418) -- (2.650000131415599,-1.1775003548221006);
\draw[line width=2pt,color=ccqqqq] (2.650000131415599,-1.1775003548221006) -- (2.665000130977559,-1.2177753497100656);
\draw[line width=2pt,color=ccqqqq] (2.665000130977559,-1.2177753497100656) -- (2.680000130539519,-1.257600344624313);
\draw[line width=2pt,color=ccqqqq] (2.680000130539519,-1.257600344624313) -- (2.6950001301014788,-1.2969753395648427);
\draw[line width=2pt,color=ccqqqq] (2.6950001301014788,-1.2969753395648427) -- (2.7100001296634386,-1.335900334531655);
\draw[line width=2pt,color=ccqqqq] (2.7100001296634386,-1.335900334531655) -- (2.7250001292253985,-1.3743753295247494);
\draw[line width=2pt,color=ccqqqq] (2.7250001292253985,-1.3743753295247494) -- (2.7400001287873583,-1.4124003245441263);
\draw[line width=2pt,color=ccqqqq] (2.7400001287873583,-1.4124003245441263) -- (2.755000128349318,-1.4499753195897858);
\draw[line width=2pt,color=ccqqqq] (2.755000128349318,-1.4499753195897858) -- (2.770000127911278,-1.4871003146617277);
\draw[line width=2pt,color=ccqqqq] (2.770000127911278,-1.4871003146617277) -- (2.785000127473238,-1.5237753097599518);
\draw[line width=2pt,color=ccqqqq] (2.785000127473238,-1.5237753097599518) -- (2.8000001270351977,-1.5600003048844584);
\draw[line width=2pt,color=ccqqqq] (2.8000001270351977,-1.5600003048844584) -- (2.8150001265971576,-1.5957753000352475);
\draw[line width=2pt,color=ccqqqq] (2.8150001265971576,-1.5957753000352475) -- (2.8300001261591174,-1.6311002952123188);
\draw[line width=2pt,color=ccqqqq] (2.8300001261591174,-1.6311002952123188) -- (2.8450001257210773,-1.6659752904156728);
\draw[line width=2pt,color=ccqqqq] (2.8450001257210773,-1.6659752904156728) -- (2.860000125283037,-1.700400285645309);
\draw[line width=2pt,color=ccqqqq] (2.860000125283037,-1.700400285645309) -- (2.875000124844997,-1.7343752809012276);
\draw[line width=2pt,color=ccqqqq] (2.875000124844997,-1.7343752809012276) -- (2.890000124406957,-1.7679002761834288);
\draw[line width=2pt,color=ccqqqq] (2.890000124406957,-1.7679002761834288) -- (2.9050001239689167,-1.8009752714919123);
\draw[line width=2pt,color=ccqqqq] (2.9050001239689167,-1.8009752714919123) -- (2.9200001235308766,-1.8336002668266782);
\draw[line width=2pt,color=ccqqqq] (2.9200001235308766,-1.8336002668266782) -- (2.9350001230928364,-1.8657752621877264);
\draw[line width=2pt,color=ccqqqq] (2.9350001230928364,-1.8657752621877264) -- (2.9500001226547963,-1.897500257575057);
\draw[line width=2pt,color=ccqqqq] (2.9500001226547963,-1.897500257575057) -- (2.965000122216756,-1.9287752529886701);
\draw[line width=2pt,color=ccqqqq] (2.965000122216756,-1.9287752529886701) -- (2.980000121778716,-1.9596002484285657);
\draw[line width=2pt,color=ccqqqq] (2.980000121778716,-1.9596002484285657) -- (2.995000121340676,-1.9899752438947438);
\draw[line width=2pt,color=ccqqqq] (2.995000121340676,-1.9899752438947438) -- (3.0100001209026357,-2.019900239387204);
\draw[line width=2pt,color=ccqqqq] (3.0100001209026357,-2.019900239387204) -- (3.0250001204645955,-2.0493752349059466);
\draw[line width=2pt,color=ccqqqq] (3.0250001204645955,-2.0493752349059466) -- (3.0400001200265554,-2.078400230450972);
\draw[line width=2pt,color=ccqqqq] (3.0400001200265554,-2.078400230450972) -- (3.0550001195885153,-2.1069752260222794);
\draw[line width=2pt,color=ccqqqq] (3.0550001195885153,-2.1069752260222794) -- (3.070000119150475,-2.1351002216198696);
\draw[line width=2pt,color=ccqqqq] (3.070000119150475,-2.1351002216198696) -- (3.085000118712435,-2.162775217243742);
\draw[line width=2pt,color=ccqqqq] (3.085000118712435,-2.162775217243742) -- (3.100000118274395,-2.1900002128938967);
\draw[line width=2pt,color=ccqqqq] (3.100000118274395,-2.1900002128938967) -- (3.1150001178363547,-2.2167752085703336);
\draw[line width=2pt,color=ccqqqq] (3.1150001178363547,-2.2167752085703336) -- (3.1300001173983145,-2.2431002042730537);
\draw[line width=2pt,color=ccqqqq] (3.1300001173983145,-2.2431002042730537) -- (3.1450001169602744,-2.2689752000020555);
\draw[line width=2pt,color=ccqqqq] (3.1450001169602744,-2.2689752000020555) -- (3.1600001165222342,-2.2944001957573397);
\draw[line width=2pt,color=ccqqqq] (3.1600001165222342,-2.2944001957573397) -- (3.175000116084194,-2.319375191538907);
\draw[line width=2pt,color=ccqqqq] (3.175000116084194,-2.319375191538907) -- (3.190000115646154,-2.343900187346756);
\draw[line width=2pt,color=ccqqqq] (3.190000115646154,-2.343900187346756) -- (3.205000115208114,-2.3679751831808877);
\draw[line width=2pt,color=ccqqqq] (3.205000115208114,-2.3679751831808877) -- (3.2200001147700736,-2.3916001790413017);
\draw[line width=2pt,color=ccqqqq] (3.2200001147700736,-2.3916001790413017) -- (3.2350001143320335,-2.414775174927998);
\draw[line width=2pt,color=ccqqqq] (3.2350001143320335,-2.414775174927998) -- (3.2500001138939933,-2.437500170840977);
\draw[line width=2pt,color=ccqqqq] (3.2500001138939933,-2.437500170840977) -- (3.265000113455953,-2.459775166780238);
\draw[line width=2pt,color=ccqqqq] (3.265000113455953,-2.459775166780238) -- (3.280000113017913,-2.481600162745782);
\draw[line width=2pt,color=ccqqqq] (3.280000113017913,-2.481600162745782) -- (3.295000112579873,-2.502975158737608);
\draw[line width=2pt,color=ccqqqq] (3.295000112579873,-2.502975158737608) -- (3.3100001121418328,-2.5239001547557165);
\draw[line width=2pt,color=ccqqqq] (3.3100001121418328,-2.5239001547557165) -- (3.3250001117037926,-2.5443751508001076);
\draw[line width=2pt,color=ccqqqq] (3.3250001117037926,-2.5443751508001076) -- (3.3400001112657525,-2.564400146870781);
\draw[line width=2pt,color=ccqqqq] (3.3400001112657525,-2.564400146870781) -- (3.3550001108277123,-2.5839751429677364);
\draw[line width=2pt,color=ccqqqq] (3.3550001108277123,-2.5839751429677364) -- (3.370000110389672,-2.6031001390909747);
\draw[line width=2pt,color=ccqqqq] (3.370000110389672,-2.6031001390909747) -- (3.385000109951632,-2.621775135240495);
\draw[line width=2pt,color=ccqqqq] (3.385000109951632,-2.621775135240495) -- (3.400000109513592,-2.6400001314162984);
\draw[line width=2pt,color=ccqqqq] (3.400000109513592,-2.6400001314162984) -- (3.4150001090755517,-2.6577751276183834);
\draw[line width=2pt,color=ccqqqq] (3.4150001090755517,-2.6577751276183834) -- (3.4300001086375116,-2.6751001238467516);
\draw[line width=2pt,color=ccqqqq] (3.4300001086375116,-2.6751001238467516) -- (3.4450001081994714,-2.6919751201014015);
\draw[line width=2pt,color=ccqqqq] (3.4450001081994714,-2.6919751201014015) -- (3.4600001077614313,-2.708400116382334);
\draw[line width=2pt,color=ccqqqq] (3.4600001077614313,-2.708400116382334) -- (3.475000107323391,-2.724375112689549);
\draw[line width=2pt,color=ccqqqq] (3.475000107323391,-2.724375112689549) -- (3.490000106885351,-2.7399001090230466);
\draw[line width=2pt,color=ccqqqq] (3.490000106885351,-2.7399001090230466) -- (3.505000106447311,-2.7549751053828264);
\draw[line width=2pt,color=ccqqqq] (3.505000106447311,-2.7549751053828264) -- (3.5200001060092707,-2.7696001017688885);
\draw[line width=2pt,color=ccqqqq] (3.5200001060092707,-2.7696001017688885) -- (3.5350001055712306,-2.7837750981812333);
\draw[line width=2pt,color=ccqqqq] (3.5350001055712306,-2.7837750981812333) -- (3.5500001051331904,-2.7975000946198603);
\draw[line width=2pt,color=ccqqqq] (3.5500001051331904,-2.7975000946198603) -- (3.5650001046951503,-2.81077509108477);
\draw[line width=2pt,color=ccqqqq] (3.5650001046951503,-2.81077509108477) -- (3.58000010425711,-2.8236000875759615);
\draw[line width=2pt,color=ccqqqq] (3.58000010425711,-2.8236000875759615) -- (3.59500010381907,-2.8359750840934357);
\draw[line width=2pt,color=ccqqqq] (3.59500010381907,-2.8359750840934357) -- (3.61000010338103,-2.8479000806371926);
\draw[line width=2pt,color=ccqqqq] (3.61000010338103,-2.8479000806371926) -- (3.6250001029429897,-2.8593750772072317);
\draw[line width=2pt,color=ccqqqq] (3.6250001029429897,-2.8593750772072317) -- (3.6400001025049495,-2.870400073803553);
\draw[line width=2pt,color=ccqqqq] (3.6400001025049495,-2.870400073803553) -- (3.6550001020669094,-2.880975070426157);
\draw[line width=2pt,color=ccqqqq] (3.6550001020669094,-2.880975070426157) -- (3.6700001016288692,-2.8911000670750435);
\draw[line width=2pt,color=ccqqqq] (3.6700001016288692,-2.8911000670750435) -- (3.685000101190829,-2.900775063750212);
\draw[line width=2pt,color=ccqqqq] (3.685000101190829,-2.900775063750212) -- (3.700000100752789,-2.9100000604516634);
\draw[line width=2pt,color=ccqqqq] (3.700000100752789,-2.9100000604516634) -- (3.715000100314749,-2.918775057179397);
\draw[line width=2pt,color=ccqqqq] (3.715000100314749,-2.918775057179397) -- (3.7300000998767087,-2.9271000539334127);
\draw[line width=2pt,color=ccqqqq] (3.7300000998767087,-2.9271000539334127) -- (3.7450000994386685,-2.934975050713711);
\draw[line width=2pt,color=ccqqqq] (3.7450000994386685,-2.934975050713711) -- (3.7600000990006284,-2.942400047520292);
\draw[line width=2pt,color=ccqqqq] (3.7600000990006284,-2.942400047520292) -- (3.7750000985625882,-2.949375044353155);
\draw[line width=2pt,color=ccqqqq] (3.7750000985625882,-2.949375044353155) -- (3.790000098124548,-2.9559000412123004);
\draw[line width=2pt,color=ccqqqq] (3.790000098124548,-2.9559000412123004) -- (3.805000097686508,-2.9619750380977283);
\draw[line width=2pt,color=ccqqqq] (3.805000097686508,-2.9619750380977283) -- (3.820000097248468,-2.967600035009439);
\draw[line width=2pt,color=ccqqqq] (3.820000097248468,-2.967600035009439) -- (3.8350000968104276,-2.9727750319474318);
\draw[line width=2pt,color=ccqqqq] (3.8350000968104276,-2.9727750319474318) -- (3.8500000963723875,-2.977500028911707);
\draw[line width=2pt,color=ccqqqq] (3.8500000963723875,-2.977500028911707) -- (3.8650000959343473,-2.9817750259022646);
\draw[line width=2pt,color=ccqqqq] (3.8650000959343473,-2.9817750259022646) -- (3.880000095496307,-2.9856000229191046);
\draw[line width=2pt,color=ccqqqq] (3.880000095496307,-2.9856000229191046) -- (3.895000095058267,-2.988975019962227);
\draw[line width=2pt,color=ccqqqq] (3.895000095058267,-2.988975019962227) -- (3.910000094620227,-2.991900017031632);
\draw[line width=2pt,color=ccqqqq] (3.910000094620227,-2.991900017031632) -- (3.9250000941821868,-2.994375014127319);
\draw[line width=2pt,color=ccqqqq] (3.9250000941821868,-2.994375014127319) -- (3.9400000937441466,-2.996400011249289);
\draw[line width=2pt,color=ccqqqq] (3.9400000937441466,-2.996400011249289) -- (3.9550000933061065,-2.9979750083975407);
\draw[line width=2pt,color=ccqqqq] (3.9550000933061065,-2.9979750083975407) -- (3.9700000928680663,-2.9991000055720756);
\draw[line width=2pt,color=ccqqqq] (3.9700000928680663,-2.9991000055720756) -- (3.985000092430026,-2.999775002772892);
\draw[line width=2pt,color=ccqqqq] (3.985000092430026,-2.999775002772892) -- (4.0000000919919865,-2.9999999999999916);
\draw[line width=2pt,color=ccqqqq] (4.0000000919919865,-2.9999999999999916) -- (4.015000091553946,-2.999774997253373);
\draw[line width=2pt,color=ccqqqq] (4.015000091553946,-2.999774997253373) -- (4.030000091115906,-2.9990999945330374);
\draw[line width=2pt,color=ccqqqq] (4.030000091115906,-2.9990999945330374) -- (4.045000090677866,-2.997974991838984);
\draw[line width=2pt,color=ccqqqq] (4.045000090677866,-2.997974991838984) -- (4.060000090239826,-2.9963999891712128);
\draw[line width=2pt,color=ccqqqq] (4.060000090239826,-2.9963999891712128) -- (4.075000089801786,-2.9943749865297242);
\draw[line width=2pt,color=ccqqqq] (4.075000089801786,-2.9943749865297242) -- (4.090000089363746,-2.991899983914518);
\draw[line width=2pt,color=ccqqqq] (4.090000089363746,-2.991899983914518) -- (4.1050000889257054,-2.988974981325594);
\draw[line width=2pt,color=ccqqqq] (4.1050000889257054,-2.988974981325594) -- (4.120000088487665,-2.9855999787629526);
\draw[line width=2pt,color=ccqqqq] (4.120000088487665,-2.9855999787629526) -- (4.135000088049625,-2.9817749762265935);
\draw[line width=2pt,color=ccqqqq] (4.135000088049625,-2.9817749762265935) -- (4.150000087611585,-2.9774999737165166);
\draw[line width=2pt,color=ccqqqq] (4.150000087611585,-2.9774999737165166) -- (4.165000087173545,-2.9727749712327225);
\draw[line width=2pt,color=ccqqqq] (4.165000087173545,-2.9727749712327225) -- (4.180000086735505,-2.9675999687752106);
\draw[line width=2pt,color=ccqqqq] (4.180000086735505,-2.9675999687752106) -- (4.195000086297465,-2.9619749663439814);
\draw[line width=2pt,color=ccqqqq] (4.195000086297465,-2.9619749663439814) -- (4.210000085859424,-2.9558999639390344);
\draw[line width=2pt,color=ccqqqq] (4.210000085859424,-2.9558999639390344) -- (4.225000085421384,-2.9493749615603697);
\draw[line width=2pt,color=ccqqqq] (4.225000085421384,-2.9493749615603697) -- (4.240000084983344,-2.9423999592079877);
\draw[line width=2pt,color=ccqqqq] (4.240000084983344,-2.9423999592079877) -- (4.255000084545304,-2.934974956881888);
\draw[line width=2pt,color=ccqqqq] (4.255000084545304,-2.934974956881888) -- (4.270000084107264,-2.9270999545820704);
\draw[line width=2pt,color=ccqqqq] (4.270000084107264,-2.9270999545820704) -- (4.285000083669224,-2.9187749523085356);
\draw[line width=2pt,color=ccqqqq] (4.285000083669224,-2.9187749523085356) -- (4.3000000832311835,-2.909999950061283);
\draw[line width=2pt,color=ccqqqq] (4.3000000832311835,-2.909999950061283) -- (4.315000082793143,-2.9007749478403126);
\draw[line width=2pt,color=ccqqqq] (4.315000082793143,-2.9007749478403126) -- (4.330000082355103,-2.891099945645625);
\draw[line width=2pt,color=ccqqqq] (4.330000082355103,-2.891099945645625) -- (4.345000081917063,-2.8809749434772196);
\draw[line width=2pt,color=ccqqqq] (4.345000081917063,-2.8809749434772196) -- (4.360000081479023,-2.870399941335097);
\draw[line width=2pt,color=ccqqqq] (4.360000081479023,-2.870399941335097) -- (4.375000081040983,-2.8593749392192565);
\draw[line width=2pt,color=ccqqqq] (4.375000081040983,-2.8593749392192565) -- (4.390000080602943,-2.8478999371296982);
\draw[line width=2pt,color=ccqqqq] (4.390000080602943,-2.8478999371296982) -- (4.4050000801649025,-2.8359749350664227);
\draw[line width=2pt,color=ccqqqq] (4.4050000801649025,-2.8359749350664227) -- (4.420000079726862,-2.8235999330294295);
\draw[line width=2pt,color=ccqqqq] (4.420000079726862,-2.8235999330294295) -- (4.435000079288822,-2.8107749310187184);
\draw[line width=2pt,color=ccqqqq] (4.435000079288822,-2.8107749310187184) -- (4.450000078850782,-2.7974999290342897);
\draw[line width=2pt,color=ccqqqq] (4.450000078850782,-2.7974999290342897) -- (4.465000078412742,-2.783774927076144);
\draw[line width=2pt,color=ccqqqq] (4.465000078412742,-2.783774927076144) -- (4.480000077974702,-2.76959992514428);
\draw[line width=2pt,color=ccqqqq] (4.480000077974702,-2.76959992514428) -- (4.495000077536662,-2.754974923238699);
\draw[line width=2pt,color=ccqqqq] (4.495000077536662,-2.754974923238699) -- (4.5100000770986215,-2.7398999213594);
\draw[line width=2pt,color=ccqqqq] (4.5100000770986215,-2.7398999213594) -- (4.525000076660581,-2.7243749195063836);
\draw[line width=2pt,color=ccqqqq] (4.525000076660581,-2.7243749195063836) -- (4.540000076222541,-2.7083999176796496);
\draw[line width=2pt,color=ccqqqq] (4.540000076222541,-2.7083999176796496) -- (4.555000075784501,-2.691974915879198);
\draw[line width=2pt,color=ccqqqq] (4.555000075784501,-2.691974915879198) -- (4.570000075346461,-2.675099914105029);
\draw[line width=2pt,color=ccqqqq] (4.570000075346461,-2.675099914105029) -- (4.585000074908421,-2.657774912357142);
\draw[line width=2pt,color=ccqqqq] (4.585000074908421,-2.657774912357142) -- (4.600000074470381,-2.6399999106355376);
\draw[line width=2pt,color=ccqqqq] (4.600000074470381,-2.6399999106355376) -- (4.6150000740323405,-2.621774908940216);
\draw[line width=2pt,color=ccqqqq] (4.6150000740323405,-2.621774908940216) -- (4.6300000735943,-2.6030999072711762);
\draw[line width=2pt,color=ccqqqq] (4.6300000735943,-2.6030999072711762) -- (4.64500007315626,-2.583974905628419);
\draw[line width=2pt,color=ccqqqq] (4.64500007315626,-2.583974905628419) -- (4.66000007271822,-2.5643999040119443);
\draw[line width=2pt,color=ccqqqq] (4.66000007271822,-2.5643999040119443) -- (4.67500007228018,-2.544374902421752);
\draw[line width=2pt,color=ccqqqq] (4.67500007228018,-2.544374902421752) -- (4.69000007184214,-2.5238999008578418);
\draw[line width=2pt,color=ccqqqq] (4.69000007184214,-2.5238999008578418) -- (4.7050000714041,-2.5029748993202143);
\draw[line width=2pt,color=ccqqqq] (4.7050000714041,-2.5029748993202143) -- (4.7200000709660594,-2.481599897808869);
\draw[line width=2pt,color=ccqqqq] (4.7200000709660594,-2.481599897808869) -- (4.735000070528019,-2.4597748963238066);
\draw[line width=2pt,color=ccqqqq] (4.735000070528019,-2.4597748963238066) -- (4.750000070089979,-2.4374998948650264);
\draw[line width=2pt,color=ccqqqq] (4.750000070089979,-2.4374998948650264) -- (4.765000069651939,-2.4147748934325284);
\draw[line width=2pt,color=ccqqqq] (4.765000069651939,-2.4147748934325284) -- (4.780000069213899,-2.391599892026313);
\draw[line width=2pt,color=ccqqqq] (4.780000069213899,-2.391599892026313) -- (4.795000068775859,-2.36797489064638);
\draw[line width=2pt,color=ccqqqq] (4.795000068775859,-2.36797489064638) -- (4.810000068337819,-2.343899889292729);
\draw[line width=2pt,color=ccqqqq] (4.810000068337819,-2.343899889292729) -- (4.825000067899778,-2.319374887965361);
\draw[line width=2pt,color=ccqqqq] (4.825000067899778,-2.319374887965361) -- (4.840000067461738,-2.294399886664275);
\draw[line width=2pt,color=ccqqqq] (4.840000067461738,-2.294399886664275) -- (4.855000067023698,-2.2689748853894716);
\draw[line width=2pt,color=ccqqqq] (4.855000067023698,-2.2689748853894716) -- (4.870000066585658,-2.2430998841409506);
\draw[line width=2pt,color=ccqqqq] (4.870000066585658,-2.2430998841409506) -- (4.885000066147618,-2.216774882918712);
\draw[line width=2pt,color=ccqqqq] (4.885000066147618,-2.216774882918712) -- (4.900000065709578,-2.189999881722756);
\draw[line width=2pt,color=ccqqqq] (4.900000065709578,-2.189999881722756) -- (4.9150000652715375,-2.1627748805530818);
\draw[line width=2pt,color=ccqqqq] (4.9150000652715375,-2.1627748805530818) -- (4.930000064833497,-2.1350998794096907);
\draw[line width=2pt,color=ccqqqq] (4.930000064833497,-2.1350998794096907) -- (4.945000064395457,-2.106974878292582);
\draw[line width=2pt,color=ccqqqq] (4.945000064395457,-2.106974878292582) -- (4.960000063957417,-2.078399877201755);
\draw[line width=2pt,color=ccqqqq] (4.960000063957417,-2.078399877201755) -- (4.975000063519377,-2.049374876137211);
\draw[line width=2pt,color=ccqqqq] (4.975000063519377,-2.049374876137211) -- (4.990000063081337,-2.019899875098949);
\draw[line width=2pt,color=ccqqqq] (4.990000063081337,-2.019899875098949) -- (5.005000062643297,-1.9899748740869698);
\draw[line width=2pt,color=ccqqqq] (5.005000062643297,-1.9899748740869698) -- (5.0200000622052565,-1.959599873101273);
\draw[line width=2pt,color=ccqqqq] (5.0200000622052565,-1.959599873101273) -- (5.035000061767216,-1.9287748721418583);
\draw[line width=2pt,color=ccqqqq] (5.035000061767216,-1.9287748721418583) -- (5.050000061329176,-1.897499871208726);
\draw[line width=2pt,color=ccqqqq] (5.050000061329176,-1.897499871208726) -- (5.065000060891136,-1.8657748703018764);
\draw[line width=2pt,color=ccqqqq] (5.065000060891136,-1.8657748703018764) -- (5.080000060453096,-1.8335998694213091);
\draw[line width=2pt,color=ccqqqq] (5.080000060453096,-1.8335998694213091) -- (5.095000060015056,-1.8009748685670242);
\draw[line width=2pt,color=ccqqqq] (5.095000060015056,-1.8009748685670242) -- (5.110000059577016,-1.7678998677390216);
\draw[line width=2pt,color=ccqqqq] (5.110000059577016,-1.7678998677390216) -- (5.1250000591389755,-1.7343748669373016);
\draw[line width=2pt,color=ccqqqq] (5.1250000591389755,-1.7343748669373016) -- (5.140000058700935,-1.700399866161864);
\draw[line width=2pt,color=ccqqqq] (5.140000058700935,-1.700399866161864) -- (5.155000058262895,-1.6659748654127087);
\draw[line width=2pt,color=ccqqqq] (5.155000058262895,-1.6659748654127087) -- (5.170000057824855,-1.6310998646898358);
\draw[line width=2pt,color=ccqqqq] (5.170000057824855,-1.6310998646898358) -- (5.185000057386815,-1.5957748639932454);
\draw[line width=2pt,color=ccqqqq] (5.185000057386815,-1.5957748639932454) -- (5.200000056948775,-1.5599998633229373);
\draw[line width=2pt,color=ccqqqq] (5.200000056948775,-1.5599998633229373) -- (5.215000056510735,-1.5237748626789116);
\draw[line width=2pt,color=ccqqqq] (5.215000056510735,-1.5237748626789116) -- (5.2300000560726945,-1.4870998620611684);
\draw[line width=2pt,color=ccqqqq] (5.2300000560726945,-1.4870998620611684) -- (5.245000055634654,-1.4499748614697077);
\draw[line width=2pt,color=ccqqqq] (5.245000055634654,-1.4499748614697077) -- (5.260000055196614,-1.4123998609045292);
\draw[line width=2pt,color=ccqqqq] (5.260000055196614,-1.4123998609045292) -- (5.275000054758574,-1.3743748603656332);
\draw[line width=2pt,color=ccqqqq] (5.275000054758574,-1.3743748603656332) -- (5.290000054320534,-1.3358998598530196);
\draw[line width=2pt,color=ccqqqq] (5.290000054320534,-1.3358998598530196) -- (5.305000053882494,-1.2969748593666885);
\draw[line width=2pt,color=ccqqqq] (5.305000053882494,-1.2969748593666885) -- (5.320000053444454,-1.2575998589066397);
\draw[line width=2pt,color=ccqqqq] (5.320000053444454,-1.2575998589066397) -- (5.3350000530064134,-1.2177748584728734);
\draw[line width=2pt,color=ccqqqq] (5.3350000530064134,-1.2177748584728734) -- (5.350000052568373,-1.1774998580653893);
\draw[line width=2pt,color=ccqqqq] (5.350000052568373,-1.1774998580653893) -- (5.365000052130333,-1.1367748576841878);
\draw[line width=2pt,color=ccqqqq] (5.365000052130333,-1.1367748576841878) -- (5.380000051692293,-1.0955998573292687);
\draw[line width=2pt,color=ccqqqq] (5.380000051692293,-1.0955998573292687) -- (5.395000051254253,-1.053974857000632);
\draw[line width=2pt,color=ccqqqq] (5.395000051254253,-1.053974857000632) -- (5.410000050816213,-1.0118998566982775);
\draw[line width=2pt,color=ccqqqq] (5.410000050816213,-1.0118998566982775) -- (5.425000050378173,-0.9693748564222058);
\draw[line width=2pt,color=ccqqqq] (5.425000050378173,-0.9693748564222058) -- (5.440000049940132,-0.926399856172416);
\draw[line width=2pt,color=ccqqqq] (5.440000049940132,-0.926399856172416) -- (5.455000049502092,-0.882974855948909);
\draw[line width=2pt,color=ccqqqq] (5.455000049502092,-0.882974855948909) -- (5.470000049064052,-0.8390998557516842);
\draw[line width=2pt,color=ccqqqq] (5.470000049064052,-0.8390998557516842) -- (5.485000048626012,-0.7947748555807421);
\draw[line width=2pt,color=ccqqqq] (5.485000048626012,-0.7947748555807421) -- (5.500000048187972,-0.7499998554360823);
\draw[line width=2pt,color=ccqqqq] (5.500000048187972,-0.7499998554360823) -- (5.515000047749932,-0.7047748553177047);
\draw[line width=2pt,color=ccqqqq] (5.515000047749932,-0.7047748553177047) -- (5.5300000473118915,-0.6590998552256098);
\draw[line width=2pt,color=ccqqqq] (5.5300000473118915,-0.6590998552256098) -- (5.545000046873851,-0.6129748551597971);
\draw[line width=2pt,color=ccqqqq] (5.545000046873851,-0.6129748551597971) -- (5.560000046435811,-0.5663998551202667);
\draw[line width=2pt,color=ccqqqq] (5.560000046435811,-0.5663998551202667) -- (5.575000045997771,-0.519374855107019);
\draw[line width=2pt,color=ccqqqq] (5.575000045997771,-0.519374855107019) -- (5.590000045559731,-0.4718998551200535);
\draw[line width=2pt,color=ccqqqq] (5.590000045559731,-0.4718998551200535) -- (5.605000045121691,-0.4239748551593703);
\draw[line width=2pt,color=ccqqqq] (5.605000045121691,-0.4239748551593703) -- (5.620000044683651,-0.37559985522496975);
\draw[line width=2pt,color=ccqqqq] (5.620000044683651,-0.37559985522496975) -- (5.6350000442456105,-0.32677485531685146);
\draw[line width=2pt,color=ccqqqq] (5.6350000442456105,-0.32677485531685146) -- (5.65000004380757,-0.2774998554350159);
\draw[line width=2pt,color=ccqqqq] (5.65000004380757,-0.2774998554350159) -- (5.66500004336953,-0.22777485557946253);
\draw[line width=2pt,color=ccqqqq] (5.66500004336953,-0.22777485557946253) -- (5.68000004293149,-0.17759985575019144);
\draw[line width=2pt,color=ccqqqq] (5.68000004293149,-0.17759985575019144) -- (5.69500004249345,-0.12697485594720304);
\draw[line width=2pt,color=ccqqqq] (5.69500004249345,-0.12697485594720304) -- (5.71000004205541,-0.07589985617049688);
\draw[line width=2pt,color=ccqqqq] (5.71000004205541,-0.07589985617049688) -- (5.72500004161737,-0.024374856420072977);
\draw[line width=2pt,color=ccqqqq] (5.72500004161737,-0.024374856420072977) -- (5.7400000411793295,0.027600143304068236);
\draw[line width=2pt,color=ccqqqq] (5.7400000411793295,0.027600143304068236) -- (5.755000040741289,0.0800251430019272);
\draw[line width=2pt,color=ccqqqq] (5.755000040741289,0.0800251430019272) -- (5.770000040303249,0.13290014267350392);
\draw[line width=2pt,color=ccqqqq] (5.770000040303249,0.13290014267350392) -- (5.785000039865209,0.18622514231879794);
\draw[line width=2pt,color=ccqqqq] (5.785000039865209,0.18622514231879794) -- (5.800000039427169,0.24000014193780972);
\draw[line width=2pt,color=ccqqqq] (5.800000039427169,0.24000014193780972) -- (5.815000038989129,0.2942251415305388);
\draw[line width=2pt,color=ccqqqq] (5.815000038989129,0.2942251415305388) -- (5.830000038551089,0.34890014109698564);
\draw[line width=2pt,color=ccqqqq] (5.830000038551089,0.34890014109698564) -- (5.8450000381130485,0.4040251406371502);
\draw[line width=2pt,color=ccqqqq] (5.8450000381130485,0.4040251406371502) -- (5.860000037675008,0.45960014015103257);
\draw[line width=2pt,color=ccqqqq] (5.860000037675008,0.45960014015103257) -- (5.875000037236968,0.5156251396386322);
\draw[line width=2pt,color=ccqqqq] (5.875000037236968,0.5156251396386322) -- (5.890000036798928,0.5721001390999492);
\draw[line width=2pt,color=ccqqqq] (5.890000036798928,0.5721001390999492) -- (5.905000036360888,0.6290251385349843);
\draw[line width=2pt,color=ccqqqq] (5.905000036360888,0.6290251385349843) -- (5.920000035922848,0.6864001379437368);
\draw[line width=2pt,color=ccqqqq] (5.920000035922848,0.6864001379437368) -- (5.935000035484808,0.7442251373262065);
\draw[line width=2pt,color=ccqqqq] (5.935000035484808,0.7442251373262065) -- (5.950000035046767,0.8025001366823941);
\draw[line width=2pt,color=ccqqqq] (5.950000035046767,0.8025001366823941) -- (5.965000034608727,0.8612251360122993);
\draw[line width=2pt,color=ccqqqq] (5.965000034608727,0.8612251360122993) -- (5.980000034170687,0.9204001353159224);
\draw[line width=2pt,color=ccqqqq] (5.980000034170687,0.9204001353159224) -- (5.995000033732647,0.9800251345932627);
\draw[line width=2pt,color=ccqqqq] (5.995000033732647,0.9800251345932627) -- (6.010000033294607,1.0401001338443203);
\draw[line width=2pt,color=ccqqqq] (6.010000033294607,1.0401001338443203) -- (6.025000032856567,1.1006251330690962);
\draw[line width=2pt,color=ccqqqq] (6.025000032856567,1.1006251330690962) -- (6.040000032418527,1.1616001322675897);
\draw[line width=2pt,color=ccqqqq] (6.040000032418527,1.1616001322675897) -- (6.055000031980486,1.2230251314398002);
\draw[line width=2pt,color=ccqqqq] (6.055000031980486,1.2230251314398002) -- (6.070000031542446,1.2849001305857284);
\draw[line width=2pt,color=ccqqqq] (6.070000031542446,1.2849001305857284) -- (6.085000031104406,1.3472251297053743);
\draw[line width=2pt,color=ccqqqq] (6.085000031104406,1.3472251297053743) -- (6.100000030666366,1.410000128798738);
\draw[line width=2pt,color=ccqqqq] (6.100000030666366,1.410000128798738) -- (6.115000030228326,1.4732251278658195);
\draw[line width=2pt,color=ccqqqq] (6.115000030228326,1.4732251278658195) -- (6.130000029790286,1.5369001269066178);
\draw[line width=2pt,color=ccqqqq] (6.130000029790286,1.5369001269066178) -- (6.1450000293522455,1.6010251259211339);
\draw[line width=2pt,color=ccqqqq] (6.1450000293522455,1.6010251259211339) -- (6.160000028914205,1.6656001249093677);
\draw[line width=2pt,color=ccqqqq] (6.160000028914205,1.6656001249093677) -- (6.175000028476165,1.7306251238713193);
\draw[line width=2pt,color=ccqqqq] (6.175000028476165,1.7306251238713193) -- (6.190000028038125,1.7961001228069886);
\draw[line width=2pt,color=ccqqqq] (6.190000028038125,1.7961001228069886) -- (6.205000027600085,1.8620251217163757);
\draw[line width=2pt,color=ccqqqq] (6.205000027600085,1.8620251217163757) -- (6.220000027162045,1.9284001205994796);
\draw[line width=2pt,color=ccqqqq] (6.220000027162045,1.9284001205994796) -- (6.235000026724005,1.9952251194563013);
\draw[line width=2pt,color=ccqqqq] (6.235000026724005,1.9952251194563013) -- (6.2500000262859645,2.0625001182868408);
\draw[line width=2pt,color=ccqqqq] (6.2500000262859645,2.0625001182868408) -- (6.265000025847924,2.130225117091098);
\draw[line width=2pt,color=ccqqqq] (6.265000025847924,2.130225117091098) -- (6.280000025409884,2.198400115869073);
\draw[line width=2pt,color=ccqqqq] (6.280000025409884,2.198400115869073) -- (6.295000024971844,2.2670251146207647);
\draw[line width=2pt,color=ccqqqq] (6.295000024971844,2.2670251146207647) -- (6.310000024533804,2.336100113346175);
\draw[line width=2pt,color=ccqqqq] (6.310000024533804,2.336100113346175) -- (6.325000024095764,2.4056251120453025);
\draw[line width=2pt,color=ccqqqq] (6.325000024095764,2.4056251120453025) -- (6.340000023657724,2.4756001107181476);
\draw[line width=2pt,color=ccqqqq] (6.340000023657724,2.4756001107181476) -- (6.3550000232196835,2.5460251093647095);
\draw[line width=2pt,color=ccqqqq] (6.3550000232196835,2.5460251093647095) -- (6.370000022781643,2.61690010798499);
\draw[line width=2pt,color=ccqqqq] (6.370000022781643,2.61690010798499) -- (6.385000022343603,2.6882251065789875);
\draw[line width=2pt,color=ccqqqq] (6.385000022343603,2.6882251065789875) -- (6.400000021905563,2.7600001051467036);
\draw[line width=2pt,color=ccqqqq] (6.400000021905563,2.7600001051467036) -- (6.415000021467523,2.8322251036881365);
\draw[line width=2pt,color=ccqqqq] (6.415000021467523,2.8322251036881365) -- (6.430000021029483,2.9049001022032863);
\draw[line width=2pt,color=ccqqqq] (6.430000021029483,2.9049001022032863) -- (6.445000020591443,2.9780251006921548);
\draw[line width=2pt,color=ccqqqq] (6.445000020591443,2.9780251006921548) -- (6.4600000201534025,3.051600099154741);
\draw[line width=2pt,color=ccqqqq] (6.4600000201534025,3.051600099154741) -- (6.475000019715362,3.125625097591044);
\draw[line width=2pt,color=ccqqqq] (6.475000019715362,3.125625097591044) -- (6.490000019277322,3.200100096001065);
\draw[line width=2pt,color=ccqqqq] (6.490000019277322,3.200100096001065) -- (6.505000018839282,3.2750250943848034);
\draw[line width=2pt,color=ccqqqq] (6.505000018839282,3.2750250943848034) -- (6.520000018401242,3.3504000927422597);
\draw[line width=2pt,color=ccqqqq] (6.520000018401242,3.3504000927422597) -- (6.535000017963202,3.426225091073433);
\draw[line width=2pt,color=ccqqqq] (6.535000017963202,3.426225091073433) -- (6.550000017525162,3.5025000893783247);
\draw[line width=2pt,color=ccqqqq] (6.550000017525162,3.5025000893783247) -- (6.565000017087121,3.5792250876569334);
\draw[line width=2pt,color=ccqqqq] (6.565000017087121,3.5792250876569334) -- (6.580000016649081,3.65640008590926);
\draw[line width=2pt,color=ccqqqq] (6.580000016649081,3.65640008590926) -- (6.595000016211041,3.734025084135304);
\draw[line width=2pt,color=ccqqqq] (6.595000016211041,3.734025084135304) -- (6.610000015773001,3.812100082335066);
\draw[line width=2pt,color=ccqqqq] (6.610000015773001,3.812100082335066) -- (6.625000015334961,3.8906250805085447);
\draw[line width=2pt,color=ccqqqq] (6.625000015334961,3.8906250805085447) -- (6.640000014896921,3.9696000786557413);
\draw[line width=2pt,color=ccqqqq] (6.640000014896921,3.9696000786557413) -- (6.655000014458881,4.049025076776656);
\draw[line width=2pt,color=ccqqqq] (6.655000014458881,4.049025076776656) -- (6.67000001402084,4.128900074871288);
\draw[line width=2pt,color=ccqqqq] (6.67000001402084,4.128900074871288) -- (6.6850000135828,4.209225072939637);
\draw[line width=2pt,color=ccqqqq] (6.6850000135828,4.209225072939637) -- (6.70000001314476,4.290000070981705);
\draw[line width=2pt,color=ccqqqq] (6.70000001314476,4.290000070981705) -- (6.71500001270672,4.371225068997489);
\draw[line width=2pt,color=ccqqqq] (6.71500001270672,4.371225068997489) -- (6.73000001226868,4.452900066986992);
\draw[line width=2pt,color=ccqqqq] (6.73000001226868,4.452900066986992) -- (6.74500001183064,4.535025064950212);
\draw[line width=2pt,color=ccqqqq] (6.74500001183064,4.535025064950212) -- (6.7600000113925995,4.617600062887149);
\draw[line width=2pt,color=ccqqqq] (6.7600000113925995,4.617600062887149) -- (6.775000010954559,4.700625060797805);
\draw[line width=2pt,color=ccqqqq] (6.775000010954559,4.700625060797805) -- (6.790000010516519,4.784100058682178);
\draw[line width=2pt,color=ccqqqq] (6.790000010516519,4.784100058682178) -- (6.805000010078479,4.868025056540268);
\draw[line width=2pt,color=ccqqqq] (6.805000010078479,4.868025056540268) -- (6.820000009640439,4.952400054372076);
\draw[line width=2pt,color=ccqqqq] (6.820000009640439,4.952400054372076) -- (6.835000009202399,5.037225052177602);
\draw[line width=2pt,color=ccqqqq] (6.835000009202399,5.037225052177602) -- (6.850000008764359,5.1225000499568445);
\draw[line width=2pt,color=ccqqqq] (6.850000008764359,5.1225000499568445) -- (6.8650000083263185,5.208225047709805);
\draw[line width=2pt,color=ccqqqq] (6.8650000083263185,5.208225047709805) -- (6.880000007888278,5.294400045436484);
\draw[line width=2pt,color=ccqqqq] (6.880000007888278,5.294400045436484) -- (6.895000007450238,5.38102504313688);
\draw[line width=2pt,color=ccqqqq] (6.895000007450238,5.38102504313688) -- (6.910000007012198,5.468100040810993);
\draw[line width=2pt,color=ccqqqq] (6.910000007012198,5.468100040810993) -- (6.925000006574158,5.555625038458825);
\draw[line width=2pt,color=ccqqqq] (6.925000006574158,5.555625038458825) -- (6.940000006136118,5.643600036080372);
\draw[line width=2pt,color=ccqqqq] (6.940000006136118,5.643600036080372) -- (6.955000005698078,5.732025033675638);
\draw[line width=2pt,color=ccqqqq] (6.955000005698078,5.732025033675638) -- (6.9700000052600375,5.820900031244623);
\draw[line width=2pt,color=ccqqqq] (6.9700000052600375,5.820900031244623) -- (6.985000004821997,5.910225028787325);
\draw[line width=2pt,color=ccqqqq] (6.985000004821997,5.910225028787325) -- (7.000000004383957,6.000000026303743);
\begin{scriptsize}
\draw[color=qqwuqq] (1.3,-3.89) node {$C_f$};
\draw[color=ccqqqq] (1.3,5.77) node {$C_g$};
\end{scriptsize}
\end{axis}
\end{tikzpicture}
\end{center}
\begin{erwthma}[resume]
\item Να βρεθεί το πεδίο ορισμού της συνάρτησης $ f\circ g $.
\item Να βρεθεί το πεδίο ορισμού της συνάρτησης $ g\circ f $.
\end{erwthma}
%# End of file DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','Basic','PdfLaTeX',NULL,1,'Γ΄ Λυκείου',2021,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','CombSub','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Συνδυαστικα θέματα/Ισότητα συναρτήσεων-Μονοτονία ακρότατα συνάρτησης-Όριο σε σημείο-Σύνθεση συναρτήσεων/Θεωρία/DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1.tex','06/5/2021 12:22','ΟΧΙ','','%# Database File : DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1
\begin{enumerate}
\item Δίνεται ένα πολυώνυμο $ P(x)=a_\nu x^\nu+a_{\nu-1}x^{\nu-1}+\ldots+a_1x+a_0 $ και $ x_0\in\mathbb{R} $. Να αποδείξετε ότι $ {\displaystyle{\lim_{x\to x_0}{P(x)}=P(x_0)}} $.
\item Να δώσετε τον ορισμό της σύνθεσης $ f\circ g $ δύο συναρτήσεων $ f $ και $ g $.
\item Δίνεται η παρακάτω πρόταση:
\begin{center}
\textit{Αν μια συνάρτηση είναι $ 1-1 $ στο πεδίο ορισμού της τότε είναι και γνησίως μονότονη.}
\end{center}
\begin{alist}
\item Να χαρακτηρίσετε την πρόταση ως αληθή (Α) ή ψευδή (Ψ).
\item Να αιτιολογήσετε την απάντησή σας.
\end{alist}
\item Δίνεται η ακόλουθη ψευδής πρόταση:
\begin{center}
\textit{Η συνάρτηση $ f(x)=\frac{1}{x} $ είναι γνησίως άυξουσα στο $ \mathbb{R}^* $.}
\end{center}
Να εντοπίσετε τα λάθη της πρότασης και να την γράψετε ξανά ωστε να είναι αληθής (μην χρησιμοποιήσετε άρνηση).
\end{enumerate}
%# End of file DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Basic','PdfLaTeX',NULL,1,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','CombSub','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Συνδυαστικα θέματα/Θεώρημα Bolzano-Μονοτονία ακρότατα συνάρτησης-Παράγωγος συνάρτηση/Θεωρία/DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1.tex','06/5/2021 14:57','ΟΧΙ','','%# Database File : DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1
\begin{alist}
\item Στο Σχήμα 1 βλέπουμε τις γραφικές παραστάσεις δύο συναρτήσεων $ f,g $. Να εξετάσετε αν ισχύει το θεώρημα {Bolzano} στα διαστήματα $ [a,\beta] $ και $ [\beta,\gamma] $ αιτιολογόντας την απάντησή σας.
\definecolor{ccqqqq}{rgb}{0.8,0,0}
\definecolor{qqwuqq}{rgb}{0,0.39215686274509803,0}
\begin{center}
\begin{tabular}{p{6.5cm}p{6.5cm}}
\begin{tikzpicture}[line cap=round,line join=round,>=triangle 45,x=1cm,y=1cm]
\begin{axis}[
x=.8cm,y=.8cm,
axis lines=middle,
xmin=-0.299999999999999,
xmax=7.26000000000001,
ymin=-2.379999999999998,
ymax=4.099999999999997,
xtick={1,2,...,7},
xticklabels={$ a $,,$ \beta $,,,$ \gamma $},
yticklabels=none]
\addplot[samples=100,domain=1:3,line width = .7mm,color=qqwuqq]{-(0.2*x-4)^2+15};
\addplot[samples=100,domain=3:6,line width = .7mm,color=ccqqqq]{-(x-4)^2+2};
\begin{scriptsize}
\draw[color=qqwuqq] (1.52,0.59) node {$C_f$};
\draw[color=ccqqqq] (3.52,1.03) node {$C_g$};
\draw[fill=white] (3,3.44) circle (.07);
\end{scriptsize}
\end{axis}
\end{tikzpicture}\captionof{figure}{Θ. \en{Bolzano}}
& \begin{tikzpicture}[line cap=round,line join=round,>=triangle 45,x=1cm,y=1cm]
\begin{axis}[
x=.8cm,y=.8cm,
axis lines=middle,
xmin=-0.299999999999999,
xmax=6.26000000000001,
ymin=-2.379999999999998,
ymax=4.099999999999997,
xtick={1,2,...,7},
xticklabels={$ a $,,$ \beta $,,$ \gamma $},
yticklabels=none]
\addplot[samples=100,domain=1:3,line width = .7mm,color=qqwuqq]{-(0.2*x-4)^2+15};
\addplot[samples=100,domain=3:5,line width = .7mm,color=qqwuqq]{(x-3)^2-2};
\begin{scriptsize}
\draw[color=qqwuqq] (1.52,0.59) node {$C_f$};
\draw[fill=white] (3,3.44) circle (.07);
\end{scriptsize}
\end{axis}
\end{tikzpicture}\captionof{figure}{Μονοτονία}
\end{tabular}
\end{center}
\item Στο Σχήμα 2 βλέπουμε τη γραφική παράσταση μιας συνάρτησης $ f $. Να εξηγήσετε γιατί η $ f $ δεν είναι συνεχής στο πεδίο ορισμού της.
\item Να δώσετε τον ορισμό του ολικού μέγιστου μιας συνάρτησης $ f $.
\item Να αποδείξετε ότι $ (x)''=1 $.
\end{alist}
%# End of file DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Basic','PdfLaTeX',NULL,1,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Anis1ou-AnisApT-SolSE1-3','SolSE','Algebra','','/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Ανισώσεις 1ου βαθμού/Λύσεις ασκήσεων ενότητας/Ανισώσεις με απόλυτες τιμές/Alg-Anis1ou-AnisApT-SolSE1-3.tex','17/7/2021 17:25','-','','%# Database File : Alg-Anis1ou-AnisApT-SolSE1-3
%@ Database source: Mathematics
s31
%# End of file Alg-Anis1ou-AnisApT-SolSE1-3
','','PdfLaTeX',NULL,0,'','','');
INSERT INTO "Database_Files" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1','SolCE','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Λυσεις συνδυαστικών ασκήσεων/Κυρτότητα - Σημεία καμπής-Μονοτονία ακρότατα συνάρτησης/Analysh-KyrtothtaMonotAkrot-SolCE2-1.tex','10/8/2021 18:43','-','','%# Database File : Analysh-KyrtothtaMonotAkrot-SolCE2-1
%@ Database source: Mathematics
λυση2-1
%# End of file Analysh-KyrtothtaMonotAkrot-SolCE2-1','','PdfLaTeX',NULL,1,'Γ΄ Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','SolCE','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Λυσεις συνδυαστικών ασκήσεων/Κυρτότητα - Σημεία καμπής-Μονοτονία ακρότατα συνάρτησης/Analysh-KyrtothtaMonotAkrot-SolCE2-1-1.tex','10/8/2021 18:43','-','','%# Database File : Analysh-KyrtothtaMonotAkrot-SolCE2-1-1
%@ Database source: Mathematics
λυση2-2
%# End of file Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','','PdfLaTeX',NULL,1,'Γ΄ Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE3-1','SolCE','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Λυσεις συνδυαστικών ασκήσεων/Κυρτότητα - Σημεία καμπής-Μονοτονία ακρότατα συνάρτησης/Analysh-KyrtothtaMonotAkrot-SolCE3-1.tex','10/8/2021 18:44','-','','%# Database File : Analysh-KyrtothtaMonotAkrot-SolCE3-1
%@ Database source: Mathematics
λυση3-1
%# End of file Analysh-KyrtothtaMonotAkrot-SolCE3-1','','PdfLaTeX',NULL,1,'Γ΄ Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE1-1','SolCE','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Λυσεις συνδυαστικών ασκήσεων/Κυρτότητα - Σημεία καμπής-Μονοτονία ακρότατα συνάρτησης/Analysh-KyrtothtaMonotAkrot-SolCE1-1.tex','10/8/2021 21:17','-','','%# Database File : Analysh-KyrtothtaMonotAkrot-SolCE1-1
%@ Database source: Mathematics
Sol1-1
%# End of file Analysh-KyrtothtaMonotAkrot-SolCE1-1','','PdfLaTeX',NULL,1,'Γ΄ Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition1','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ορισμοί/DTX-Pithanothtes-Pithan-DeigmEnd-Definition1.tex','17/9/2021 11:43','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-Definition1
%@ Database source: Mathematics
Πείραμα τύχης ονομάζεται κάθε πείραμα του οποίου το αποτέλεσμα δεν μπορεί να προβλεφθεί με απόλυτη βεβαιότητα όσες φορές κι αν αυτό επαναληφθεί, κάτω από τις ίδιες συνθήκες.
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-Definition1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition2','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ορισμοί/DTX-Pithanothtes-Pithan-DeigmEnd-Definition2.tex','17/9/2021 11:54','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-Definition2
%@ Database source: Mathematics
Δειγματικός χώρος ονομάζεται το σύνολο το οποίο περιέχει όλα τα πιθανά αποτελέσματα ενός πειράματος τύχης. Ο δειγματικός αποτελεί βασικό σύνολο. \[ \varOmega=\left\lbrace \omega_1,\omega_2,\ldots,\omega_\nu \right\rbrace \]
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-Definition2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition3','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ορισμοί/DTX-Pithanothtes-Pithan-DeigmEnd-Definition3.tex','17/9/2021 11:55','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-Definition3
%@ Database source: Mathematics
Δίνεται o δειγματικός χώρος $ \varOmega=\{\omega_1,\omega_2,\ldots,\omega_{\nu}\} $ ενός πειράματος τύχης. Ενδεχόμενο ονομάζεται οποιοδήποτε σύνολο $A$ το οποίο περιέχει ένα ή περισσότερα στοιχεία του δειγματικού χώρου.
\begin{itemize}[itemsep=0mm]
\item Κάθε ενδεχόμενο είναι υποσύνολο του δειγματικού του χώρου.
\item Τα ενδεχόμενα που έχουν ένα στοιχείο ονομάζονται \textbf{απλά} ενδεχόμενα, ενώ αν περιέχουν περισσότερα στοιχεία ονομάζονται \textbf{σύνθετα}.
\item Εάν το αποτέλεσμα ενός πειράματος είναι στοιχείο ενός ενδεχομένου τότε λέμε ότι το ενδεχόμενο \textbf{πραγματοποιείται}.
\item Τα στοιχεία ενός ενδεχομένου ονομάζονται \textbf{ευνοϊκές περιπτώσεις}.
\item Ο δειγματικός χώρος $ \varOmega $ ονομάζεται \textbf{βέβαιο} ενδεχόμενο, ενώ το κενό σύνολο ονομάζεται \textbf{αδύνατο} ενδεχόμενο.
\item Εάν δύο ενδεχόμενα $ A,B $ δεν έχουν κοινά στοιχεία τότε ονομάζονται \textbf{ασυμβίβαστα} ή ξένα μεταξύ τους δηλαδή : \[ A,B \textrm{ ασυμβίβαστα }\Leftrightarrow A\cap B=\varnothing \]
\end{itemize}
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-Definition3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition1','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Η έννοια της πιθανότητας/Ορισμοί/DTX-Pithanothtes-Pithan-Pithanothta-Definition1.tex','17/9/2021 12:15','-','','%# Database File : DTX-Pithanothtes-Pithan-Pithanothta-Definition1
%@ Database source: Mathematics
Πιθανότητα ενός ενδεχομένου $ A=\{a_1,a_2,\ldots,a_\kappa\} $ ενός δειγματικού χώρου $ \varOmega $ ονομάζεται ο λόγος του πλήθους των ευνοϊκών περιπτώσεων του $ A $ προς το πλήθος όλων των δυνατών περιπτώσεων.
\[ P(A)=\frac{N(A)}{N(\varOmega)} \]
\begin{itemize}[itemsep=0mm]
\item Ο παραπάνω ορισμός ονομάζεται \textbf{κλασικός ορισμός} της πιθανότητας και εφαρμόζεται όταν το ενδεχόμενο $ A $ αποτελείται από ισοπίθανα απλά ενδεχόμενα $ \{a_i\}\ ,\ i=1,2,\ldots,\kappa $.
\item Το πλήθος των στοιχείων ενός ενδεχομένου $ A $ συμβολίζεται με $ N(A) $.
\end{itemize}
%# End of file DTX-Pithanothtes-Pithan-Pithanothta-Definition1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition2','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Η έννοια της πιθανότητας/Ορισμοί/DTX-Pithanothtes-Pithan-Pithanothta-Definition2.tex','17/9/2021 12:17','-','','%# Database File : DTX-Pithanothtes-Pithan-Pithanothta-Definition2
%@ Database source: Mathematics
Η πιθανότητα ενός ενδεχομένου $ A=\{a_1,a_2,\ldots,a_\kappa\} $ ενός δειγματικού χώρου $ \varOmega=\{\omega_1,\omega_2,\ldots,\omega_\nu\} $ ορίζεται ώς το άθροισμα των πιθανοτήτων $ P(a_i)\ ,\ i=1,2,\ldots,\nu $ των απλών ενδεχομένων του.
\[ P(A)=P(a_1)+P(a_2)+\ldots+P(a_\kappa) \]
\begin{itemize}[itemsep=0mm]
\item Για κάθε στοιχείο $ \omega_i\ ,\ i=1,2,\ldots,\nu $ του δειγματικού χώρου $ \varOmega $ ονομάζουμε τον αριθμό $ P(\omega_i) $ πιθανότητα του ενδεχομένου $ \{\omega_i\} $.
\item Ο παραπάνω ορισμός ονομάζεται \textbf{αξιωματικός ορισμός} της πιθανότητας και εφαρμόζεται όταν το ενδεχόμενο $ A $ δεν αποτελείται από ισοπίθανα απλά ενδεχόμενα $ \{a_i\}\ ,\ i=1,2,\ldots,\kappa $.
\end{itemize}
%# End of file DTX-Pithanothtes-Pithan-Pithanothta-Definition2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem1','Theor','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Η έννοια της πιθανότητας/Θεωρήματα/DTX-Pithanothtes-Pithan-Pithanothta-Theorem1.tex','17/9/2021 12:19','-','','%# Database File : DTX-Pithanothtes-Pithan-Pithanothta-Theorem1
%@ Database source: Mathematics
Από τον κλασικό ορισμό της πιθανότητας προκύπτουν οι παρακάτω ιδιότητες :
\begin{rlist}
\item Πιθανότητα κενού συνόλου : $ P(\varnothing)=0 $.
\item Πιθανότητα δειγματικού χώρου : $ P(\varOmega)=1 $.
\item Για κάθε ενδεχόμενο $ A $ ισχύει : $ 0\leq P(A)\leq1 $.
\end{rlist}
%# End of file DTX-Pithanothtes-Pithan-Pithanothta-Theorem1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem2','Theor','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Η έννοια της πιθανότητας/Θεωρήματα/DTX-Pithanothtes-Pithan-Pithanothta-Theorem2.tex','17/9/2021 23:30','-','','%# Database File : DTX-Pithanothtes-Pithan-Pithanothta-Theorem2
%@ Database source: Mathematics
Οι παρακάτω ιδιότητες μας δείχνουν τις σχέσεις με τις οποίες συνδέονται οι πιθανότητες οποιονδήποτε ενδεχομένων $ A,B $ με τις πιθανότητες των ενδεχομένων των πράξεων που περιέχουν τα ενδεχόμενα αυτά.
\begin{center}
\begin{tabular}{cc}
\hline \rule[-2ex]{0pt}{5.5ex} \textbf{Ενδεχόμενο} & \textbf{Πιθανότητα} \\ 
\hhline{==} \rule[-2ex]{0pt}{7.5ex} Ένωση & $ P(A\cup B)=\ccases{P(A)+P(B)-P(A\cap B)\ \ ,\ \ \textrm{αν }A\cap B\neq\varnothing\\
P(A)+P(B)\ \ ,\ \ \textrm{αν }A\cap B=\varnothing} $ \\ 
\rule[-2ex]{0pt}{5.5ex} Συμπλήρωμα & $ P(A'')=1-P(A) $ \\ 
\hhline{~-}\rule[-2ex]{0pt}{5.5ex} \multirow{3}{*}{Διαφορά} & $ P(A-B)=P(A)-P(A\cap B) $ \\ 
\rule[-2ex]{0pt}{5.5ex}  & $ P(B-A)=P(B)-P(A\cap B) $ \\ 
\hhline{~-}\rule[-2ex]{0pt}{5.5ex} Υποσύνολο & $ A\subseteq B\Rightarrow P(A)\leq P(B) $ \\ 
\hline 
\end{tabular} 
\end{center}
%# End of file DTX-Pithanothtes-Pithan-Pithanothta-Theorem2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition1','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Definition1.tex','20/9/2021 17:13','-','','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Definition1
%@ Database source: Mathematics
Πληθυσμός ονομάζεται ένα σύνολο όμοιων στοιχείων τα οποία εξετάζονται ως προς ένα ή περισσότερα χαρακτηριστικά. Το πλήθος των στοιχείων ενός πληθυσμού ονομάζεται \textbf{μέγεθος} του πληθυσμού.
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Definition1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition2','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Definition2.tex','20/9/2021 23:11','-','','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Definition2
%@ Database source: Mathematics
Δείγμα ονομάζεται ένα υποσύνολο ενός πληθυσμού. \begin{itemize}
\item Ένα δείγμα λέγεται \textbf{αντιπροσωπευτικό} ενός πληθυσμού όταν τα συμπεράσματα που προκύπτουν από τη μελέτη του είναι αρκετά αξιόπιστα ώστε να μπορούν να γενικευτούν για ολόκληρο τον πληθυσμό.
\item Το πλήθος των στοιχείων ενός δείγματος ονομάζεται \textbf{μέγεθος} του δείγματος.
\end{itemize}
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Definition2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition3','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Definition3.tex','20/9/2021 23:13','-','','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Definition3
%@ Database source: Mathematics
Μεταβλητή ονομάζεται το χαρακτηριστικό ως προς το οποίο εξετάζονται τα στοιχεία ενός πληθυσμού.
\begin{itemize}
\item Συμβολίζεται με οποιοδήποτε κεφαλαίο γράμμα : $ X,Y,A,B,\ldots $
\item Οι πιθανές τιμές οι οποίες μπορεί να πάρει μια μεταβλητή ονομάζονται \textbf{τιμές της μεταβλητής}. Συμβολίζονται με το ίδιο μικρό γράμμα του ονόματος της μεταβλητής π.χ. $ x_i,y_i\ldots $ όπου ο δείκτης $ i $ φανερώνει τον αύξοντα αριθμό της τιμής.
\item Τα στατιστικά δεδομένα που συλλέγονται από ένα πληθυσμό ή δείγμα που εξετάζεται ως προς κάποια μεταβλητή ονομάζονται \textbf{παρατηρήσεις}. Συμβολίζονται συνήθως με $ t_i $ όπου ο δείκτης $ i $ φανερώνει τον αύξοντα αριθμό της παρατήρησης.
\end{itemize} 
Οι μεταβλητές διακρίνονται στις εξής κατηγορίες :
\begin{enumerate}[label=\bf\arabic*.]
\item \textbf{Ποιοτικές}\\
Ποιοτική ονομάζεται κάθε μεταβλητή της οποίας οι τιμές δεν είναι αριθμητικές.
\item \textbf{Ποσοτικές}\\
Ποσοτική ονομάζεται κάθε μεταβλητή της οποίας οι τιμές είναι αριθμοί. Οι ποσοτικές μεταβλητές χωρίζονται σε διακριτές και συνεχείς.
\begin{rlist}
\item \textbf{Διακριτές} ονομάζονται οι ποσοτικές μεταβλητές που παίρνουν μεμονωμένες τιμές από το σύνολο των πραγματικών αριθμών ή ένα διάστημα αυτού.
\item \textbf{Συνεχείς} ονομάζονται οι ποσοτικές μεταβλητές που παίρνουν όλες τις τιμές στο σύνολο ή σε ένα διάστημα πραγματικών αριθμών.
\end{rlist}
\end{enumerate}
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Definition3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem3','Theor','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Η έννοια της πιθανότητας/Θεωρήματα/DTX-Pithanothtes-Pithan-Pithanothta-Theorem3.tex','20/9/2021 23:32','-','','%# Database File : DTX-Pithanothtes-Pithan-Pithanothta-Theorem3
%@ Database source: Mathematics
Μεταξύ των πιθανοτήτων δύο οποιονδήποτε ενδεχομένων $ A,B $ ενός δειγματικού χώρου $ \varOmega $ καθώς και των ενδεχομένων που προκύπτουν από πράξεις που τα περιέχουν, ισχύουν οι ακόλουθες ανισότητες.
\begin{multicols}{3}
\begin{rlist}
\item $ P(A)\leq P(A\cup B) $
\item $ P(B)\leq P(A\cup B) $
\item $ P(A\cap B)\leq P(A) $
\item $ P(A\cap B)\leq P(B) $
\item $ P(A\cap B)\leq P(A\cup B) $
\item $ P(A-B)\leq P(A) $
\item $ P(B-A)\leq P(B) $
\item $ P(A-B)\leq P(A\cup B) $
\item $ P(B-A)\leq P(A\cup B) $
\end{rlist}
\end{multicols}
%# End of file DTX-Pithanothtes-Pithan-Pithanothta-Theorem3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-Method1','Method','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Μέθοδοι/DTX-Algebra-Syst-GrammikaSys-Method1.tex','20/9/2021 23:38','-','','%# Database File : DTX-Algebra-Syst-GrammikaSys-Method1
%@ Database source: Mathematics
Δεδομένης μιας γραμμικής εξίσωσης μπορούμε να εξετάσουμε ποια σημεία ανήκουν στην ευθεία που παριστάνει καθώς και τα σημεία τομής της ευθείας αυτής με τους άξονες.
\begin{itemize}[leftmargin=3mm]
\item \textbf{Σημεία ευθείας}\\
Για να εξετάσουμε αν κάποιο σημείο $ A(x_0,y_0) $ ανήκει σε μια ευθεία, αντικαθιστούμε τις συντεταγμένες του στη θέση των μεταβλητών της εξίσωσης και εξετάζουμε αν αυτή επαληθεύεται. Αν ναι τότε το σημείο ανήκει στην ευθεία.
\item \textbf{Σημεία τομής με τους άξονες}\\
Για να βρούμε τα σημεία τομής μιας ευθείας με τους άξονες τότε θέτουμε όπου $ x=0 $ ή $ y=0 $ στη γραμμική εξίσωση για τους άξονες $ y''y $ και $ x''x $ αντίστοιχα.
\end{itemize}
%# End of file DTX-Algebra-Syst-GrammikaSys-Method1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-Example1','Example','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Λυμένα παραδείγματα/DTX-Algebra-Syst-GrammikaSys-Example1.tex','20/9/2021 23:40','-','','%# Database File : DTX-Algebra-Syst-GrammikaSys-Example1
%@ Database source: Mathematics
\bmath{Δίνεται η ευθεία με εξίσωση $ 2x+y=7 $.
\begin{alist}
\item Να εξεταστεί αν τα σημεία $ A(2,3) $ και $ B(-1,4) $ ανήκουν στην ευθεία.
\item Να βρεθεί η τιμή της παραμέτρου $ \lambda $ αν γνωρίζουμε ότι το σημείο $ \varGamma(\lambda,2\lambda+3) $ ανήκει στην ευθεία.
\end{alist}}
\lysh
\begin{alist}
\item Αντικαθιστούμε τς συντεταγμένες του σημείου $ A(2,3) $ στην εξίσωση και έχουμε:
\begin{align*}
\textrm{Για }x=2\ \textrm{και }y=3&\Rightarrow 2\cdot 2+3=7\Rightarrow\\&\Rightarrow 4+3=7\Rightarrow 7=7
\end{align*}
Η εξίσωση επαληθεύεται οπότε το σημείο $ A $ ανήκει στην ευθεία. Ομοίως για το σημείο $ B $ θα έχουμε :
\[ \textrm{Για }x=-1\ \textrm{και }y=4\Rightarrow 2\cdot (-1)+4=7\Rightarrow -2+4=7\Rightarrow 2=7 \]
Η εξίσωση δεν επαληθεύεται οπότε το σημείο $ B $ δεν ανήκει στην ευθεία.
\item Αφού το σημείο $ \varGamma $ ανήκει στην ευθεία τότε οι συντεταγμένες του επαληθεύουν της εξίσωση της. Έτσι για $ x=\lambda $ και $ y=2\lambda+3 $ έχουμε:
\[ 2\cdot\lambda+2\lambda+3=7\Rightarrow 4\lambda=4\Rightarrow\lambda=1 \]
\end{alist}
%# End of file DTX-Algebra-Syst-GrammikaSys-Example1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition1','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα διασποράς/Ορισμοί/DTX-Statistikh-MetThDias-MetraDiasp-Definition1.tex','21/9/2021 00:15','-','','%# Database File : DTX-Statistikh-MetThDias-MetraDiasp-Definition1
%@ Database source: Mathematics
Εύρος παρατηρήσεων ενός δείγματος ονομάζεται η διαφορά μεταξύ μέγιστης και ελάχιστης παρατήρησης. Συμβολίζεται με $ R $ και είναι :
\[ R=t_{max}-t_{min} \]
%# End of file DTX-Statistikh-MetThDias-MetraDiasp-Definition1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','CombEx','Geometria',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Συνδυαστικές ασκήσεις/Εμβαδά βασικών σχημάτων-Πυθαγόρειο θεώρημα - Ορθές προβολές/DTX-Geometria-EmbadaBasSxPythTheor-CombEx1.tex','21/9/2021 12:20','NO','','%# Database File : DTX-Geometria-EmbadaBasSxPythTheor-CombEx1
%@ Database source: Mathematics
\wrapr{-5mm}{10}{3.9cm}{-7mm}{
\begin{tikzpicture}[scale=.7]
\draw  (-4,4) rectangle (0.5,-2);
\draw (-4,1) -- (0.5,1);
\draw  (-1.75,3.2) ellipse (0.6 and 0.6);
\draw[fill=white]  (-3,4) rectangle (-0.5,3);
\draw[fill=white]  (-2.5,4) rectangle (-1,3.5);
\draw  (-1.75,-1.2) ellipse (0.6 and 0.6);
\draw[fill=white]   (-3,-2) rectangle (-0.5,-1);
\draw[fill=white]   (-2.5,-2) rectangle (-1,-1.5);
\draw  (-1.75,1) ellipse (0.6 and 0.6);
\node at (-4.3,4.2){A};
\node at (-4.2,-2.2) {B};
\node at (0.75,-2.2) {$\Gamma$};
\draw (-4,4) -- (0.5,-2);
\end{tikzpicture}}{
Το στάδιο Camp Nou βρίσκεται στην πόλη της Βαρκελώνης στην Ισπανία και είναι έδρα της ποδοσφαιρικής ομάδας FC Barcelona. Το μήκος $ AB $ του ποδοσφαιρικού γηπέδου είναι $ 105m $. Δίνεται γνωστό ότι η διαγώνια απόσταση $ A\varGamma $ ανάμεσα σε δύο απέναντι corner είναι $ 125{,}1m $.
\begin{rlist}
\item Πόσο είναι το πλάτος $ B\varGamma $ του γηπέδου;
\item Να βρεθεί το εμβαδόν που καταλαμβάνει η επιφάνεια του γηπέδου.
\item Αν γνωρίζουμε ότι το κόστος τοποθέτησης του χλοοτάπητα είναι 12\officialeuro\, για κάθε τ.μ. να βρεθεί το συνολικό κόστος τοποθέτησής του.
\end{rlist}}
%# End of file DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','Basic','PdfLaTeX',NULL,1,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1','SectEx','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Προβλήματα/DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1.tex','21/9/2021 23:06','NO','','%# Database File : DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1
%@ Database source: Mathematics
Ο μεγάλος υπερτυχερός του λαχείου μόλις κέρδισε $ 4.500.000 $\officialeuro\,  στον πρώτο λαχνό. Τα χρήματα αυτά θα τα παραλάβει από την τράπεζα σε $ 650 $ δεσμίδες των $ 50 $\officialeuro\, και $ 100 $\officialeuro\,. Αν κάθε δεσμίδα περιέχει $ 100 $ χαρτονομίσματα πόσες δεσμίδες από κάθε είδος χαρτονομίσματος θα χρειαστούν;
%# End of file DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1','SectEx','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Η έννοια της πιθανότητας/Ασκήσεις/Προβλήματα/DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1.tex','21/9/2021 23:16','NO','','%# Database File : DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1
%@ Database source: Mathematics
Ο καθηγητής των μαθηματικών σε ένα λύκειο πρόκειται να επιλέξει μαθητές από όλες τις τάξεις για να εκπροσωπήσουν το σχολείο στη διεθνή Μαθηματική Ολυμπιάδα του \the\year{}. Θα πρέπει να λάβει υπ όψιν του το αν ο μαθητής είναι αγόρι (α) ή κορίτσι (κ), την τάξη στην οποία πηγαίνει (Α΄, Β΄ ή Γ΄) και το αν έχει συμμετάσχει ξανά (ναι : (ν) ή όχι (ο) ) σε οποιονδήποτε διαγωνισμό μαθηματικών.
\begin{rlist}
\item Να βρεθεί ο δειγματικός χώρος του πειράματος.
\item Αν ο καθηγητής επιλέξει τυχαία έναν μαθητή να βρεθεί το ενδεχόμενο
\begin{enumerate}[itemsep=0mm]
\item[Α :] Ο μαθητής να είναι αγόρι.
\item[Β :] Ο μαθητής να ανήκει σε κάποια ομάδα προσανατολισμού και να μην έχει συμμετάσχει ξανά σε διαγωνισμό
\item[Γ :] Ο μαθητής να είναι κορίτσι και να έχει συμμετάσχει ξανά σε μαθηματικό διαγωνισμό.
\end{enumerate}
\item Να υπολογιστούν οι πιθανότητες των παραπάνω ενδεχομένων Α, Β, Γ.
\end{rlist}
%# End of file DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition4','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ορισμοί/DTX-Pithanothtes-Pithan-DeigmEnd-Definition4.tex','22/9/2021 10:34','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-Definition4
%@ Database source: Mathematics
Ένωση δύο ενδεχομένων $ A,B $ ονομάζεται το ενδεχόμενο το οποίο περιέχει τα κοινά και μη κοινά στοιχεία των δύο ενδεχομένων. Η ένωση πραγματοποιείται όταν πραγματοποιείται τουλάχιστον ένα από τα ενδεχόμενα $ A $ ή $ B $. \[ x\in A\cup B\Leftrightarrow x\in A \textrm{ ή }x\in B \]
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-Definition4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition5','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ορισμοί/DTX-Pithanothtes-Pithan-DeigmEnd-Definition5.tex','22/9/2021 10:34','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-Definition5
%@ Database source: Mathematics
Τομή δύο ενδεχομένων $ A,B $ ονομάζεται το ενδεχόμενο το οποίο περιέχει τα κοινά στοιχεία των δύο ενδεχομένων. Η τομή πραγματοποιείται όταν πραγματοποιούνται συγχρόνως και τα δύο ενδεχόμενα $ A $ και $ B $. \[ x\in A\cap B\Leftrightarrow x\in A \textrm{ και }x\in B \]
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-Definition5','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition6','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ορισμοί/DTX-Pithanothtes-Pithan-DeigmEnd-Definition6.tex','22/9/2021 10:34','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-Definition6
%@ Database source: Mathematics
Συμπλήρωμα ενός ενδεχομένου $ A $ ονομάζεται το ενδεχόμενο το οποίο περιέχει τα στοιχεία εκείνα τα οποία \textbf{δεν} ανήκουν στο σύνολο $ A $. Το συμπλήρωμα πραγματοποιείται όταν δεν πραγματοποιείται το $ A $. \[ x\in A''\Leftrightarrow x\notin A\]
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-Definition6','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition7','Def','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ορισμοί/DTX-Pithanothtes-Pithan-DeigmEnd-Definition7.tex','22/9/2021 10:35','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-Definition7
%@ Database source: Mathematics
Διαφορά ενός ενδεχομένου $ A $ από ένα ενδεχόμενο $ B $ ονομάζεται το ενδεχόμενο που περιέχει τα στοιχεία που ανήκουν μόνο στο ενδεχόμενο $ A $. Η διαφορά πραγματοποιείται όταν πραγματοποιείται μόνο το ενδεχόμενο $ A $. \[ x\in A-B\Leftrightarrow x\in A \textrm{ και }x\notin B \]
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-Definition7','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Theorem1','Theor','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Θεωρήματα/DTX-Statistikh-PaStD-BasEnnStat-Theorem1.tex','22/9/2021 10:36','-','','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Theorem1
%@ Database source: Mathematics
Οι πίνακες στους οποίους συγκεντρώνουμε τα στατιστικά δεδομένα καθώς και πληροφορίες που μας βοηθούν να εξάγουμε συμπεράσματα για το δείγμα ή πληθυσμό ονομάζονται στατιστικοί πίνακες. Οι κατηγορίες πινάκων είναι :
\begin{enumerate}[label=\bf\arabic*.,itemsep=0mm]
\item \textbf{Γενικοί πίνακες}\\
Οι γενικοί πίνακες περιέχουν αναλυτικά όλες τις πληροφορίες που αφορούν τα δεδομένα που συλλέξαμε.
\item \textbf{Ειδικοί πίνακες}\\
Οι ειδικοί πίνακες είναι συνοπτικοί και περιέχουν πληροφορίες από τους γενικούς πίνακες.
\end{enumerate}
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Theorem1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition1','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Παρουσίαση στατιστικών δεδομένων/Ορισμοί/DTX-Statistikh-PaStD-ParStD-Definition1.tex','22/9/2021 10:40','-','','%# Database File : DTX-Statistikh-PaStD-ParStD-Definition1
%@ Database source: Mathematics
Έστω $ X $ μια μεταβλητή με τιμές $ x_1,x_2,\ldots,x_{\kappa} $. Απόλυτη συχνότητα ή συχνότητα μιας τιμής $ x_i,\ i=1,2,\ldots,\kappa $, ονομάζεται ο φυσικός αριθμός $ \nu_i $ ο οποίος μας δίνει το πλήθος των εμφανίσεων της τιμής αυτής μέσα στο δείγμα.
%# End of file DTX-Statistikh-PaStD-ParStD-Definition1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition2','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Παρουσίαση στατιστικών δεδομένων/Ορισμοί/DTX-Statistikh-PaStD-ParStD-Definition2.tex','22/9/2021 10:43','-','','%# Database File : DTX-Statistikh-PaStD-ParStD-Definition2
%@ Database source: Mathematics
Έστω $ X $ μια μεταβλητή με τιμές $ x_1,x_2,\ldots,x_{\kappa} $. Σχετική συχνότητα μιας τιμής $ x_i $ ονομάζεται ο λόγος $ f_i=\frac{\nu_i}{\nu} $ ο οποίος μας δίνει το ποσοστό εμφάνισης της τιμής ως μέρος του δείγματος. Μπορεί να εκφραστεί και ως ποσοστό επί τοις $ 100 $ και είναι \[ f_i\%=\frac{\nu_i}{\nu}\cdot 100\% \]
%# End of file DTX-Statistikh-PaStD-ParStD-Definition2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition3','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Παρουσίαση στατιστικών δεδομένων/Ορισμοί/DTX-Statistikh-PaStD-ParStD-Definition3.tex','22/9/2021 10:47','-','','%# Database File : DTX-Statistikh-PaStD-ParStD-Definition3
%@ Database source: Mathematics
Δίνεται $ X $ μια ποσοτική μεταβλητή με τιμές $ x_1,x_2,\ldots,x_{\kappa} $, οι οποίες είναι τοποθετημένες σε αύξουσα σειρά. Αθροιστική συχνότητα μιας τιμής $ x_i,\ i=1,2,\ldots,\kappa $, ονομάζεται ο φυσικός αριθμός $ N_i $ ο οποίος μας δίνει το πλήθος των παρατηρήσεων που είναι μικρότερες ή ίσες της τιμής $ x_i $. Δίνεται από τον τύπο:
\[ N_i=\nu_1+\nu_2+\ldots+\nu_i \]
%# End of file DTX-Statistikh-PaStD-ParStD-Definition3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition4','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Παρουσίαση στατιστικών δεδομένων/Ορισμοί/DTX-Statistikh-PaStD-ParStD-Definition4.tex','22/9/2021 10:51','-','','%# Database File : DTX-Statistikh-PaStD-ParStD-Definition4
%@ Database source: Mathematics
Δίνεται $ X $ μια ποσοτική μεταβλκτή με τιμές $x_1,x_2,\ldots,x_{\kappa}$, τοποθετημένες σε αύξουσα σειρά. Σχετική αθροιστική συχνότητα μιας τιμής $ x_i,i=1,2,\ldots,\kappa $, ονομάζεται ο φυσικός αριθμός $ F_i $ ο οποίος μας δίνει το ποσοστό των παρατηρήσεων που είναι μικρότερες ή ίσες την τιμή $ x_i $. Δίνεται από τον τύπο:
\[ F_i=f_1+f_2+\ldots+f_i \]
Υπολογίζεται μόνο για ποσοτικές μεταβλητές. Μπορεί να εκφραστεί και ως ποσοστό επί τοις $ 100 $ και είναι $ F_i\%=F_i\cdot 100\% $.
%# End of file DTX-Statistikh-PaStD-ParStD-Definition4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition1','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα θέσης/Ορισμοί/DTX-Statistikh-MetThDias-MetraThesis-Definition1.tex','22/9/2021 10:58','-','','%# Database File : DTX-Statistikh-MetThDias-MetraThesis-Definition1
%@ Database source: Mathematics
Η μέση τιμή ορίζεται ως το πηλίκο του αθροίσματος των παρατηρήσεων ενός δείγματος προς το πλήθος τους. Συμβολίζεται $ \bar{x} $ και είναι :
\[ \bar{x}=\frac{t_1+t_2+\ldots+t_\nu}{\nu}=\frac{1}{\nu}\sum_{i=1}^{\nu}{t_i} \]
Εναλλακτικοί τύποι για τη μέση τιμή είναι οι ακόλουθοι οι οποίοι χρησιμοποιούνται σε κατανομές συχνοτήτων. Αν κάποια μεταβλητή $ X $ έχει τιμές $ x_1,x_2\ldots,x_\kappa $ με συχνότητες $ \nu_1,\nu_2\ldots,\nu_\kappa $ και σχετικές συχνότητες $ f_1,f_2,\ldots,f_\kappa $ τότε θα έχουμε :
\[ \bar{x}=\frac{1}{\nu}\sum_{i=1}^{\kappa}{x_i\nu_i}\ \textrm{ και }\ \bar{x}=\sum_{i=1}^{\kappa}{x_if_i} \]
%# End of file DTX-Statistikh-MetThDias-MetraThesis-Definition1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition2','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα θέσης/Ορισμοί/DTX-Statistikh-MetThDias-MetraThesis-Definition2.tex','22/9/2021 11:00','-','','%# Database File : DTX-Statistikh-MetThDias-MetraThesis-Definition2
%@ Database source: Mathematics
Ο σταθμικός μέσος ορίζεται ως η μέση τιμή των παρατηρήσεων όταν αυτές έχουν ξεχωριστό συντελεστή βαρύτητας. Ισούται με 
\[ \bar{x}=\frac{t_iw_1+t_2w_2+\ldots+t_\nu w_\nu}{w_1+w_2+\ldots+w_\nu}=\frac{\sum\limits_{i=1}^{\nu}{t_iw_i}}{\sum\limits_{i=1}^{\nu}{w_i}} \]
όπου $ w_i\ ,\ i=1,2,\ldots,\nu $ είναι οι συντελεστές βαρύτητας των παρατηρήσεων.
%# End of file DTX-Statistikh-MetThDias-MetraThesis-Definition2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition2','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα διασποράς/Ορισμοί/DTX-Statistikh-MetThDias-MetraDiasp-Definition2.tex','22/9/2021 11:01','-','','%# Database File : DTX-Statistikh-MetThDias-MetraDiasp-Definition2
%@ Database source: Mathematics
Η τυπική απόκλιση ορίζεται ως η θετική τετραγωνική ρίζα της διακύμανσης.
\[ s=\sqrt{s^2} \]
%# End of file DTX-Statistikh-MetThDias-MetraDiasp-Definition2','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition3','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα διασποράς/Ορισμοί/DTX-Statistikh-MetThDias-MetraDiasp-Definition3.tex','22/9/2021 11:01','-','','%# Database File : DTX-Statistikh-MetThDias-MetraDiasp-Definition3
%@ Database source: Mathematics
Συντελεστής μεταβολής ή μεταβλητότητας ονομάζεται ο λόγος της τυπικής απόκλισης προς την απόλυτη τιμή του μέσου όρου του δείγματος. Συμβολίζεται $ CV $.
\[ CV=\frac{s}{|x|}\cdot 100\% \]
\begin{itemize}
\item Μας δίνει την ομοιογένεια των δεδομένων ενός δείγματος.
\item Ένα δείγμα χαρακτηρίζεται ομοιογενές αν ο συντελεστής μεταβολής του είναι μικρότερος του $ 10\% $.
\end{itemize}
%# End of file DTX-Statistikh-MetThDias-MetraDiasp-Definition3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition4','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα διασποράς/Ορισμοί/DTX-Statistikh-MetThDias-MetraDiasp-Definition4.tex','22/9/2021 11:04','-','','%# Database File : DTX-Statistikh-MetThDias-MetraDiasp-Definition4
%@ Database source: Mathematics
Διακύμανση ονομάζεται η μέση τιμή των τετραγώνων των διαφορών των παρατηρήσεων $ t_i $ από τη μέση τιμή $ \bar{x} $ τους. Συμβολίζεται με $ s^2 $.
\[ s^2=\frac{1}{\nu}\sum_{i=1}^{\nu}{(t_i-\bar{x})^2} \]
Σε μια κατανομή συχνοτήτων αν μια μεταβλητή έχει τιμές $ x_1,x_2,\ldots,x_\kappa $ με συχνότητες $ \nu_1,\nu_2,\ldots,\nu_\kappa $ και σχετικές συχνότητες $ f_1,f_2,\ldots,f_\kappa $ τότε η διακύμανση δίνεται από τους παρακάτω τύπους :
\begin{multicols}{2}
\begin{rlist}
\item $ s^2=\dfrac{1}{\nu}\LEFTRIGHT\{\}{\sum\limits_{i=1}^{\nu}{t_i^2}-\frac{\left( \sum\limits_{i=1}^{\nu}{t_i}\right)^2 }{\nu}} $
\item $ s^2=\frac{1}{\nu}\sum\limits_{i=1}^{\kappa}{(x_i-\bar{x})^2\nu_i} $
\item $ s^2=\dfrac{1}{\nu}\LEFTRIGHT\{\}{\sum\limits_{i=1}^{\kappa}{x_i^2\nu_i}-\frac{\left( \sum\limits_{i=1}^{\kappa}{x_i\nu_i}\right)^2 }{\nu}} $
\item $ s^2=\sum\limits_{i=1}^{\kappa}{(x_i-\bar{x})^2 f_i} $
\end{rlist}
\end{multicols}
\begin{rlist}[start=5]
\item $ s^2=\sum\limits_{i=1}^{\kappa}{x_i^2f_i}-\bar{x}^2 $
\item $ s^2=\overline{x^2}-\bar{x}^2 $ όπου $ \overline{x^2}=\frac{1}{\nu}\sum\limits_{i=1}^{\nu}{t_i^2}=\frac{1}{\nu}\sum\limits_{i=1}^{\kappa}{x_i^2\nu_i}=\sum\limits_{i=1}^{\kappa}{x_i^2f_i} $
\end{rlist}
%# End of file DTX-Statistikh-MetThDias-MetraDiasp-Definition4','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition3','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα θέσης/Ορισμοί/DTX-Statistikh-MetThDias-MetraThesis-Definition3.tex','22/9/2021 11:05','-','','%# Database File : DTX-Statistikh-MetThDias-MetraThesis-Definition3
%@ Database source: Mathematics
Διάμεσος ονομάζεται η κεντρική παρατήρηση $ \nu $ σε πλήθους παρατηρήσεων όταν αυτές έχουν τοποθετηθεί σε αύξουσα σειρά. Συμβολίζεται με $ \delta $. Ξεχωρίζουμε τις εξής περιπτώσεις :
\begin{rlist}
\item Αν το πλήθος των $ \nu $ παρατηρήσεων είναι περιττό τότε η διάμεσος ισούται με τη μεσαία παρατήρηση.
\[ \delta=t_{_{\frac{\nu}{2}}} \]
\item Αν το πλήθος των $ \nu $ παρατηρήσεων είναι άρτιο τότε η διάμεσος ισούται με το ημιάθροισμα των δύο μεσαίων παρατηρήσεων.
\[ \delta=\frac{t_{_{\frac{\nu}{2}}}+t_{_{\frac{\nu}{2}+1}}}{2} \]
\end{rlist}
Η διάμεσος σε κατανομή συχνοτήτων ισούται με την τιμή $ x_i $ για την οποία η σχετική αθροιστική συχνότητα $ F_i\% $ είτε ισούται είτε ξεπερνάει για πρώτη φορά το $ 50\% $. Δηλαδή
\[ \delta=x_i\ \textrm{ για την οποία }\ F_{i-1}\%<50\%\leq F_i\% \]
%# End of file DTX-Statistikh-MetThDias-MetraThesis-Definition3','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1','SectEx','Pithanothtes',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Ασκήσεις/Πράξεις ενδεχομένων/DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1.tex','22/9/2021 11:24','YES','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1
%@ Database source: Mathematics
Σε κάποιο λύκειο επιλέγουμε τυχαία έναν μαθητή και εξετάζουμε γι αυτόν τα εξής ενδεχόμενα.
\begin{enumerate}
\item[Α:] Ο μαθητής παίζει ποδόσφαιρο
\item[Β:] Ο μαθητής παίζει μπάσκετ
\end{enumerate}
Να δώσετε σε καθένα από τα παρακάτω ενδεχόμενα μια λεκτική περιγραφή.
\begin{multicols}{3}
\begin{alist}
\item $ A\cup B $
\item $ A\cap B $
\item $ A'' $
\item $ B'' $
\item $ A-B $
\item $ B-A $
\end{alist}
\end{multicols}
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Arith-Diataxh-Definition1','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αριθμοί/Διάταξη αριθμών/Ορισμοί/DTX-Algebra-Arith-Diataxh-Definition1.tex','22/9/2021 19:42','-','','%# Database File : DTX-Algebra-Arith-Diataxh-Definition1
%@ Database source: Mathematics
Κλειστό διάστημα ονομάζεται το σύνολο των πραγματικών αριθμών που βρίσκονται μεταξύ δύο αριθμών $ a,\beta\in\mathbb{R} $. Συμβολίζεται με $ [a,\beta] $.
\[ [a,\beta]=\{x\in\mathbb{R}|a\leq x\leq \beta\} \]
\begin{itemize}[itemsep=0mm]
\item Οι $ a,\beta $ ονομάζονται \textbf{άκρα} του διαστήματος.
\item Κάθε διάστημα μπορεί να εκφραστεί σαν ανισότητα και αντίστροφα.
\item Αν από το κλειστό διάστημα παραλείψουμε τα άκρα $ a,\beta $ τό διάστημα που προκύπτει ονομάζεται \textbf{ανοιχτό διάστημα} $ (a,\beta) $.
\item Το σύνολο των πραγματικών αριθμών $ x\geq a $ ορίζουν το διάστημα $ [a,+\infty) $. Ομοίως, τα διαστήματα $ (a,\infty), (-\infty,a] $ και $ (-\infty,a) $ είναι τα σύνολα των αριθμών $ x $ για τους οποίους ισχύει αντίστοιχα $ x>a,x\leq a $ και $ x<a $.
\item Ο αριθμός $ x_0=\frac{a+\beta}{2} $ ονομάζεται \textbf{κέντρο}, ο αριθμός $ \mu=\beta-a $ ονομάζεται \textbf{μήκος} και ο αριθμός $ \rho=\frac{\beta-a}{2} $ ονομάζεται \textbf{ακτίνα} του διαστήματος.
\end{itemize}
Στον παρακάτω πίνακα βλέπουμε όλους τους τύπους διαστημάτων, τη γραφική παράστασή τους καθώς και το πως παριστάνεται το καθένα σαν ανισότητα.
\begin{center}
\begin{tabular}{cc>{\centering\arraybackslash}m{4cm}c}
\hline \rule[-2ex]{0pt}{5.5ex} \textbf{Διάστημα} & \textbf{Ανισότητα} & \textbf{Σχήμα} & \textbf{Περιγραφή} \\ 
\hhline{====} \rule[-2ex]{0pt}{5.5ex} $ [a,\beta] $ & $ a\leq x\leq\beta $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\Diasthma{a}{ \beta }{.7}{2.3}{.3}{\xrwma!30}
\Axonas{0}{3}
\Akro{k}{.7}
\Akro{k}{2.3}
\end{tikzpicture} & Κλειστό $ a,\beta $ \\ 
$ (a,\beta) $ & $ a< x<\beta $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\Diasthma{a}{ \beta }{.7}{2.3}{.3}{\xrwma!30}
\Axonas{0}{3}
\Akro{a}{.7}
\Akro{a}{2.3}
\end{tikzpicture} & Ανοιχτό $ a,\beta $\\
$ [a,\beta) $ & $ a\leq x<\beta $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\Diasthma{a}{ \beta }{.7}{2.3}{.3}{\xrwma!30}
\Axonas{0}{3}
\Akro{k}{.7}
\Akro{a}{2.3}
\end{tikzpicture} & Κλειστό $a$ ανοιχτό $\beta$\\
$ (a,\beta] $ & $ a< x\leq\beta $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\Diasthma{a}{ \beta }{.7}{2.3}{.3}{\xrwma!30}
\Axonas{0}{3}
\Akro{a}{.7}
\Akro{k}{2.3}
\end{tikzpicture} & Ανοιχτό $a$ κλειστό $\beta$ \\
$ [a,+\infty) $ & $ x\geq a $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\Xapeiro{a}{.7}{3}{.3}{\xrwma!30}
\Axonas{0}{3}
\Akro{k}{.7}
\end{tikzpicture} & Κλειστό $a$ συν άπειρο \\
$ (a,+\infty) $ & $ x>a $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\Xapeiro{a}{.7}{3}{.3}{\xrwma!30}
\Axonas{0}{3}
\Akro{a}{.7}
\end{tikzpicture} & Ανοιχτό $a$ συν άπειρο \\
$ (-\infty,a] $ & $ x\leq a $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\ApeiroX{a}{2.3}{0}{.35}{\xrwma!30}
\Axonas{0}{3}
\Akro{k}{2.3}
\end{tikzpicture} & Μείον άπειρο $a$ κλειστό \\
$ (-\infty,a) $ & $ x<a $ & \begin{tikzpicture}
\tkzDefPoint(0,.57){A}
\ApeiroX{a}{2.3}{0}{.35}{\xrwma!30}
\Axonas{0}{3}
\Akro{a}{2.3}
\end{tikzpicture} & Μείον άπειρο $a$ ανοιχτό \\
\hline 
\end{tabular}
\end{center}
%# End of file DTX-Algebra-Arith-Diataxh-Definition1','Basic','PdfLaTeX',NULL,0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Akol-Akolouth-Definition1','Def','Algebra',3,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ακολουθίες/Ακολουθίες/Ορισμοί/DTX-Algebra-Akol-Akolouth-Definition1.tex','27/9/2021 16:52','-','','Κείμενο
%# Database File : DTX-Algebra-Akol-Akolouth-Definition1
%@ Database source: Mathematics
Δοκιμή
%# End of file DTX-Algebra-Akol-Akolouth-Definition1
Τέλος κειμένου','Basic','PdfLaTeX','Περιγραφή της νέας άσκησης. Εδώ μπαίνει το νέο κείμενο',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Akol-ArProodos-Definition1','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ακολουθίες/Αριθμητική πρόοδος/Ορισμοί/DTX-Algebra-Akol-ArProodos-Definition1.tex','13/10/2021 11:40','-','','%# Database File : DTX-Algebra-Akol-ArProodos-Definition1
%@ Database source: Mathematics
test 13-10
%# End of file DTX-Algebra-Akol-ArProodos-Definition1','Askhseis','PdfLaTeX','test',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SolSE1-1','SolSE','Pithanothtes','','/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Πιθανότητες/Πιθανότητες/Δειγματικός χώρος - Ενδεχόμενα/Λύσεις ασκήσεων ενότητας/Πράξεις ενδεχομένων/DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SolSE1-1.tex','13/10/2021 11:44','-','','%# Database File : DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SolSE1-1
%@ Database source: Mathematics
Πρόχειρη λύση
%# End of file DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE2-1','SolSE','Algebra','','/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Λύσεις ασκήσεων ενότητας/Παραμετρικές/Alg-GrammikaSys-Parametrikes-SolSE2-1.tex','27/10/2021 01:41','-','','%# Database File : Alg-GrammikaSys-Parametrikes-SolSE2-1
%@ Database source: Mathematics
Η εξίσωση παριστάνει ευθεία αν ισχύει
\begin{itemize}
\item $\lambda^2-1\neq 0\Rightarrow \lambda^2\neq 1\Rightarrow \lambda\neq\pm 1$ ή
\item $1-\lambda\neq 0\Rightarrow \lambda\neq 1$
\end{itemize}
άρα $ \lambda\neq 1 $. Το σημείο $ A(1,3) $ ανήκει στην ευθεία αν και μόνο αν
\[ \left(\lambda^2-1\right)\cdot 1+(1-\lambda)\cdot 3=2\Rightarrow \lambda^2-1+3-3\lambda-2=0\Rightarrow \lambda^2-3\lambda=0\Rightarrow \lambda=0\ \text{ή}\ \lambda=3 \]
%# End of file Alg-GrammikaSys-Parametrikes-SolSE2-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Akol-Akolouth-Definition2','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ακολουθίες/Ακολουθίες/Ορισμοί/DTX-Algebra-Akol-Akolouth-Definition2.tex','29/10/2021 16:49','-','@article{DTX-Algebra-Akol-Akolouth-Definition2,}','%# Database File : DTX-Algebra-Akol-Akolouth-Definition2
%@ Database source: Mathematics
csv
%# End of file DTX-Algebra-Akol-Akolouth-Definition2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','CombEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συνδυαστικές ασκήσεις/Ακολουθίες-Ανισώσεις 1ου βαθμού/DTX-Algebra-AkolouthAnis1ou-CombEx1.tex','29/10/2021 17:31','NO','@article{DTX-Algebra-AkolouthAnis1ou-CombEx1,}','%# Database File : DTX-Algebra-AkolouthAnis1ou-CombEx1
%@ Database source: Mathematics
csv2
%# End of file DTX-Algebra-AkolouthAnis1ou-CombEx1','Basic','PdfLaTeX','',1,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ακολουθίες/Ακολουθίες/Ασκήσεις/Αναδρομικός τύπος ακολουθίας/DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1.tex','10/11/2021 02:03','NO','@article{DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1,}','%# Database File : DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1
%@ Database source: Mathematics
c
%# End of file DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Akol-Akolouth-Prob1','Prob','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ακολουθίες/Ακολουθίες/Προβλήματα/DTX-Algebra-Akol-Akolouth-Prob1.tex','11/11/2021 00:38','NO','@article{DTX-Algebra-Akol-Akolouth-Prob1,}','%# Database File : DTX-Algebra-Akol-Akolouth-Prob1
%@ Database source: Mathematics
d
%# End of file DTX-Algebra-Akol-Akolouth-Prob1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1','SectEx','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος γινομένου/DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1.tex','29/11/2021 12:22','NO','@article{DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1,}','%# Database File : DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1
%@ Database source: Mathematics
Να βρεθούν οι παράγωγοι των παρακάτω συναρτήσεων
\begin{enumerate}
\item $ f(x)=x^2\cdot\hm{x} $
\item $ f(x)=x^3\cdot\sqrt{x} $
\item $ f(x)=\syn{x}\cdot\hm{x} $
\item $ f(x)=x\cdot\ef{x} $
\end{enumerate}
%# End of file DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1','SectEx','Analysh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Παράγωγος συνάρτηση/Ασκήσεις/Παράγωγος πηλίκου/DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1.tex','29/11/2021 12:28','NO','@article{DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1,}','%# Database File : DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1
%@ Database source: Mathematics
Να βρεθούν οι παράγωγοι των παρακάτω συναρτήσεων
\begin{enumerate}
\item $ f(x)=\frac{\hm{x}}{x} $
\item $ f(x)=\frac{x+1}{x-2} $
\item $ f(x)=\frac{2x-1}{x^2} $
\item $ f(x)=\frac{\syn{x}}{\sqrt{x}} $
\end{enumerate}
%# End of file DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Synart-AntistrSyn-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Η έννοια της συνάρτησης - Βασικές συναρτήσεις/Αντίστροφη συνάρτηση/Ορισμοί/DTX-Analysh-Synart-AntistrSyn-Def1.tex','08/12/2021 12:55','-','@article{DTX-Analysh-Synart-AntistrSyn-Def1,}','%# Database File : DTX-Analysh-Synart-AntistrSyn-Def1
%@ Database source: Mathematics
Έστω μια συνάρτηση $ f:A\to\mathbb{R} $ με σύνολο τιμών $ f(A) $. Η συνάρτηση με την οποία κάθε $ y\in f(A) $ αντιστοιχεί σε ένα \textbf{μοναδικό} $ x\in A $ για το οποίο ισχύει $ f(x)=y $, λέγεται αντίστροφη συνάρτηση της $ f $.
\begin{center}
\begin{tikzpicture}[scale=.6]
\draw(0,0) ellipse (1cm and 1.5cm);
\draw(4,0) ellipse (1cm and 1.5cm);
\draw[fill=\xrwma!50] (4.1,0) ellipse (.6cm and 1.1cm);
\draw[latex-] (0,.2) arc (140:40:2.6);
\tkzDefPoint(0,.2){A}
\tkzDefPoint(4,.2){B}
\tkzDrawPoints(A,B)
\tkzLabelPoint[left](A){{\footnotesize $ x $}}
\tkzLabelPoint[right](B){{\footnotesize $ y $}}
\tkzText(0,1.8){$ A $}
\tkzText(4,1.8){$ B $}
\tkzText(2,1.45){$ f^{-1} $}
\draw[-latex] (3.5,0) -- (2.7,-1) node[anchor=north east] {\footnotesize $ f\left( A \right)  $};
\end{tikzpicture}
\end{center}
\begin{itemize}[itemsep=0mm]
\item Συμβολίζεται με $ f^{-1} $ και είναι $ f^{-1}:f(A)\to A $.
\item Το πεδίο ορισμού της $ f^{-1} $ είναι το σύνολο τιμών $ f(A) $ της $ f $, ενώ το σύνολο τιμών της $ f^{-1} $ είναι το πεδίο ορισμού $ A $ της $ f $.
\item Ισχύει ότι $ x=f^{-1}(y) $ για κάθε $ y\in f(A) $.
\end{itemize}
%# End of file DTX-Analysh-Synart-AntistrSyn-Def1','Basic','PdfLaTeX','Αντίστροφη συνάρτηση',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Κυρτότητα - Σημεία καμπής/Ορισμοί/DTX-Analysh-Parag-Kyrtothta-Def1.tex','08/12/2021 13:05','-','@article{DTX-Analysh-Parag-Kyrtothta-Def1,}','%# Database File : DTX-Analysh-Parag-Kyrtothta-Def1
%@ Database source: Mathematics
Μια συνάρτηση $ f $ λέμε ότι στρέφει τα κοίλα προς τα άνω ή είναι κυρτή στο $ \Delta $, αν η $ f'' $ είναι γνησίως αύξουσα στο $ \Delta $.
%# End of file DTX-Analysh-Parag-Kyrtothta-Def1','Basic','PdfLaTeX','Κυρτή συνάρτηση',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def2','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Κυρτότητα - Σημεία καμπής/Ορισμοί/DTX-Analysh-Parag-Kyrtothta-Def2.tex','08/12/2021 13:06','-','@article{DTX-Analysh-Parag-Kyrtothta-Def2,}','%# Database File : DTX-Analysh-Parag-Kyrtothta-Def2
%@ Database source: Mathematics
Μια συνάρτηση $ f $ λέμε ότι στρέφει τα κοίλα προς τα κάτω ή είναι κοίλη στο $ \Delta $, αν η $ f'' $ είναι γνησίως φθίνουσα στο $ \Delta $.
%# End of file DTX-Analysh-Parag-Kyrtothta-Def2','Basic','PdfLaTeX','Κοίλη συνάρτηση',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def3','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Κυρτότητα - Σημεία καμπής/Ορισμοί/DTX-Analysh-Parag-Kyrtothta-Def3.tex','08/12/2021 13:07','-','@article{DTX-Analysh-Parag-Kyrtothta-Def3,}','%# Database File : DTX-Analysh-Parag-Kyrtothta-Def3
%@ Database source: Mathematics
Έστω μια συνάρτηση $ f $ παραγωγίσιμη σε ένα διάστημα $ (a,\beta) $, με εξαίρεση ίσως ένα σημείο $ x_0 $. Αν:
\begin{itemize}[itemsep=0mm]
\item η $ f $ είναι κυρτή στο $ (a,x_0) $ και κοίλη στο $ (x_0,\beta) $ η αντιστρόφως και
\item η $ C_f $ έχει εφαπτομένη στο $ A(x_0,f(x_0)) $
\end{itemize}
τότε το σημείο $ A(x_0,f(x_0)) $ λέγεται \textbf{σημείο καμπής} της $ C_f $.
%# End of file DTX-Analysh-Parag-Kyrtothta-Def3','Basic','PdfLaTeX','Σημείο καμπής',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Oria-Asymptotes-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Όρια/Ασύμπτωτες/Ορισμοί/DTX-Analysh-Oria-Asymptotes-Def1.tex','08/12/2021 13:15','-','@article{DTX-Analysh-Oria-Asymptotes-Def1,}','%# Database File : DTX-Analysh-Oria-Asymptotes-Def1
%@ Database source: Mathematics
Αν ένα τουλάχιστον από τα όρια $ \lim\limits_{x\to x_0^-}{f(x)},\lim\limits_{x\to x_0^+}{f(x)} $ ισούται με $ \pm\infty $ τότε η ευθεία $ x=x_0 $ λέγεται \textbf{κατακόρυφη ασύμπτωτη} της $ C_f $.
%# End of file DTX-Analysh-Oria-Asymptotes-Def1','Basic','PdfLaTeX','Κατακόρυφη ασύμπτωτη',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Oria-Asymptotes-Def2','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Όρια/Ασύμπτωτες/Ορισμοί/DTX-Analysh-Oria-Asymptotes-Def2.tex','08/12/2021 13:16','-','@article{DTX-Analysh-Oria-Asymptotes-Def2,}','%# Database File : DTX-Analysh-Oria-Asymptotes-Def2
%@ Database source: Mathematics
Αν $ \lim\limits_{x\to +\infty}{f(x)}=l $ (αντιστοίχως $ \lim\limits_{x\to -\infty}{f(x)}=l $) τότε η ευθεία $ y=l $ λέγεται \textbf{οριζόντια ασύμπτωτη} της $ C_f $ στο $ +\infty $ (αντίστοιχα στο $ -\infty $).
%# End of file DTX-Analysh-Oria-Asymptotes-Def2','Basic','PdfLaTeX','Οριζόντια ασύμπτωτη',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Oria-Asymptotes-Def3','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Όρια/Ασύμπτωτες/Ορισμοί/DTX-Analysh-Oria-Asymptotes-Def3.tex','08/12/2021 13:17','-','@article{DTX-Analysh-Oria-Asymptotes-Def3,}','%# Database File : DTX-Analysh-Oria-Asymptotes-Def3
%@ Database source: Mathematics
Η ευθεία $ y=\lambda x+\beta $ λέγεται ασύμπτωτη της $ C_f $ στο $ +\infty $ (αντιστοίχως στο $ -\infty $) αν και μόνο αν
\[ \lim\limits_{x\to +\infty}{[f(x)-(\lambda x+\beta)]=0} \]
αντίστοιχα στο $ -\infty $ αν 
\[ \lim_{x\to -\infty}{[f(x)-(\lambda x+\beta)]=0} \]
\begin{itemize}[itemsep=0mm]
\item Αν $ \lambda=0 $ η ασύμπτωτη είναι οριζόντια.
\item Αν $ \lambda\neq 0 $ η ασύμπτωτη είναι πλάγια.
\end{itemize}
%# End of file DTX-Analysh-Oria-Asymptotes-Def3','Basic','PdfLaTeX','Ασύμπτωτη',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Olokl-OrismOlokl-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Ολοκληρώματα/Ορισμένο ολοκλήρωμα/Ορισμοί/DTX-Analysh-Olokl-OrismOlokl-Def1.tex','08/12/2021 17:17','-','@article{DTX-Analysh-Olokl-OrismOlokl-Def1,}','%# Database File : DTX-Analysh-Olokl-OrismOlokl-Def1
%@ Database source: Mathematics
Το ορισμένο ολοκλήρωμα μιας συνεχούς συνάρτησης $ f $݂ από το $ a $ στο $ \beta $ είναι το όριο
\[ \lim _{\nu \rightarrow+\infty}\left(\sum_{\kappa=1}^{\nu} f\left(\xi_{\kappa}\right) \Delta x\right) \text { όπου } \Delta x=\frac{\beta-\alpha}{\nu} \]
όταν αυτό υπάρχει στο $ \mathbb{R} $, όπου $ f(x_\xi) $ είναι μια τιμή της $ f $ στο $ [x_{\kappa-1},x_{\kappa}] $ για κάθε $ \xi_{\kappa}\in[x_{\kappa-1},x_{\kappa}],\ \kappa=1,2,3,\ldots,\nu $. Το ορισμένο ολοκλήρωμα συμβολίζεται με 
\[ \int_{a}^{\beta}f(x)dx \]
και η συνάρτηση $ f $ λέγεται ολοκληρώσιμη στο διάστημα $ [a,\beta] $. Δηλαδή 
\[ \int_{a}^{\beta}f(x)dx=\lim _{\nu \rightarrow+\infty}\left(\sum_{\kappa=1}^{\nu} f\left(\xi_{\kappa}\right) \Delta x\right) \]
%# End of file DTX-Analysh-Olokl-OrismOlokl-Def1','Basic','PdfLaTeX','Ορισμένο ολοκλήρωμα',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Μονοτονία ακρότατα συνάρτησης/Ορισμοί/DTX-Analysh-Parag-MonotAkrot-Def1.tex','09/12/2021 11:51','-','@article{DTX-Analysh-Parag-MonotAkrot-Def1,}','%# Database File : DTX-Analysh-Parag-MonotAkrot-Def1
%@ Database source: Mathematics
Έστω μια συνάρτηση $ f $ ορισμένη σε ένα διάστημα $ \Delta $. Οι πιθανές θέσεις των τοπικών ακροτάτων της είναι
\begin{enumerate}
\item Τα εσωτερικά σημεία $ x_0\in\Delta $ στα οποία η παράγωγος μηδενίζεται.
\item Τα εσωτερικά σημεία $ x_0\in\Delta $ στα οποία η παράγωγος δεν ορίζεται.
\item Τα άκρα του διαστήματος $ \Delta $ αν αυτό είναι κλειστό.
\end{enumerate}
%# End of file DTX-Analysh-Parag-MonotAkrot-Def1','Basic','PdfLaTeX','Πιθανές θέσεις τοπικών ακροτάτων',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def2','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Μονοτονία ακρότατα συνάρτησης/Ορισμοί/DTX-Analysh-Parag-MonotAkrot-Def2.tex','09/12/2021 11:52','-','@article{DTX-Analysh-Parag-MonotAkrot-Def2,}','%# Database File : DTX-Analysh-Parag-MonotAkrot-Def2
%@ Database source: Mathematics
Έστω μια συνάρτηση $ f $ ορισμένη σε ένα διάστημα $ \Delta $. Τα κρίσιμα σημεία της είναι
\begin{enumerate}
\item Τα εσωτερικά σημεία $ x_0\in\Delta $ στα οποία η παράγωγος μηδενίζεται.
\item Τα εσωτερικά σημεία $ x_0\in\Delta $ στα οποία η παράγωγος δεν ορίζεται.
\end{enumerate}
%# End of file DTX-Analysh-Parag-MonotAkrot-Def2','Basic','PdfLaTeX','Κρίσιμα σημεία',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def4','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Κυρτότητα - Σημεία καμπής/Ορισμοί/DTX-Analysh-Parag-Kyrtothta-Def4.tex','09/12/2021 11:56','-','@article{DTX-Analysh-Parag-Kyrtothta-Def4,}','%# Database File : DTX-Analysh-Parag-Kyrtothta-Def4
%@ Database source: Mathematics
Έστω μια συνάρτηση $ f $ ορισμένη σε ένα διάστημα $ \Delta $. Οι πιθανές θέσεις των σημείων καμπής της $ C_f $ είναι
\begin{enumerate}
\item Τα εσωτερικά σημεία $ x_0\in\Delta $ στα οποία η $ f'''' $ μηδενίζεται.
\item Τα εσωτερικά σημεία $ x_0\in\Delta $ στα οποία η $ f'''' $ δεν ορίζεται, με την προϋπόθεση ότι στα σημεία αυτά ορίζεται εφαπτομένη.
\end{enumerate}
%# End of file DTX-Analysh-Parag-Kyrtothta-Def4','Basic','PdfLaTeX','Πιθανές θέσεις σημείων καμπής',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Synart-Synarthsh-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Η έννοια της συνάρτησης - Βασικές συναρτήσεις/Η έννοια της συνάρτησης/Ορισμοί/DTX-Analysh-Synart-Synarthsh-Def1.tex','16/12/2021 22:41','-','@article{DTX-Analysh-Synart-Synarthsh-Def1,}','%# Database File : DTX-Analysh-Synart-Synarthsh-Def1
%@ Database source: Mathematics
Συνάρτηση είναι μια διαδικασία (κανόνας) με την οποία κάθε στοιχείο ενός συνόλου $A$ αντιστοιχίζεται σε ένα ακριβώς στοιχείο κάποιου άλλου συνόλου $B$.
%# End of file DTX-Analysh-Synart-Synarthsh-Def1','Basic','PdfLaTeX','Η έννοια της συνάρτησης',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def3','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Μονοτονία ακρότατα συνάρτησης/Ορισμοί/DTX-Analysh-Parag-MonotAkrot-Def3.tex','16/12/2021 22:49','-','@article{DTX-Analysh-Parag-MonotAkrot-Def3,}','%# Database File : DTX-Analysh-Parag-MonotAkrot-Def3
%@ Database source: Mathematics
Μια συνάρτηση $f$ με πεδίο ορισμού το $ A $ λέμε ότι παρουσιάζει τοπικό μέγιστο στο $ x_0\in A $, όταν $ f(x)\leq f(x_0) $ για κάθε x σε μια περιοχή του $ x_0 $.
%# End of file DTX-Analysh-Parag-MonotAkrot-Def3','Basic','PdfLaTeX','Τοπικό μέγιστο',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def4','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Μονοτονία ακρότατα συνάρτησης/Ορισμοί/DTX-Analysh-Parag-MonotAkrot-Def4.tex','16/12/2021 22:51','-','@article{DTX-Analysh-Parag-MonotAkrot-Def4,}','%# Database File : DTX-Analysh-Parag-MonotAkrot-Def4
%@ Database source: Mathematics
Μια συνάρτηση $f$ με πεδίο ορισμού το $ A $ λέμε ότι παρουσιάζει τοπικό ελάχιστο στο $ x_0\in A $, όταν $ f(x)\geq f(x_0) $ για κάθε x σε μια περιοχή του $ x_0 $.
%# End of file DTX-Analysh-Parag-MonotAkrot-Def4','Basic','PdfLaTeX','Τοπικό ελάχιστο',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-ParagShmeio-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Παράγωγος σε σημείο/Ορισμοί/DTX-Analysh-Parag-ParagShmeio-Def1.tex','16/12/2021 22:56','-','@article{DTX-Analysh-Parag-ParagShmeio-Def1,}','%# Database File : DTX-Analysh-Parag-ParagShmeio-Def1
%@ Database source: Mathematics
Μια συνάρτηση $ f $ με πεδίο ορισμού $ A $ θα λέγεται παραγωγίσιμη σε ένα σημείο $ x_0 $ του πεδίου ορισμού της, όταν το όριο
\[ \lim_{h\to 0}\frac{f(x_0+h)-f(x_0)}{h} \]
υπάρχει και είναι πραγματικός αριθμός. Το όριο αυτό ονομάζεται παράγωγος της $ f $ στο $ x_0 $ και συμβολίζεται με $ f''(x_0) $. Δηλαδή
\[ f''(x_0)=\lim_{h\to 0}\frac{f(x_0+h)-f(x_0)}{h} \]
%# End of file DTX-Analysh-Parag-ParagShmeio-Def1','Basic','PdfLaTeX','Παράγωγος σε σημείο',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-RythMetab-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Ρυθμός μεταβολής/Ορισμοί/DTX-Analysh-Parag-RythMetab-Def1.tex','16/12/2021 23:00','-','@article{DTX-Analysh-Parag-RythMetab-Def1,}','%# Database File : DTX-Analysh-Parag-RythMetab-Def1
%@ Database source: Mathematics
Η παράγωγος της $f$ στο $x_0\in A$ εκφράζει το ρυθμό μεταβολής του $ y=f(x) $ ως προς το $x$, όταν $ x=x_0 $.
%# End of file DTX-Analysh-Parag-RythMetab-Def1','Basic','PdfLaTeX','Ρυθμός μεταβολής',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Paragogos-Def1','Def','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Παράγωγος συνάρτηση/Ορισμοί/DTX-Analysh-Parag-Paragogos-Def1.tex','16/12/2021 23:03','-','@article{DTX-Analysh-Parag-Paragogos-Def1,}','%# Database File : DTX-Analysh-Parag-Paragogos-Def1
%@ Database source: Mathematics
Έστω μια συνάρτηση $f$ με πεδίο ορισμού το $Α$, και $Β$ το σύνολο των $x\in A$ στα οποία η $f$ είναι παραγωγίσιμη. Τότε ορίζεται μια νέα συνάρτηση, με την οποία κάθε $x\in B$ αντιστοιχίζεται στο 
\[ f''(x)=\lim_{h\to 0}\frac{f(x+h)-f(x)}{h} \]
Η συνάρτηση αυτή λέγεται (πρώτη) παράγωγος της $f$ και συμβολίζεται με $f''$.
%# End of file DTX-Analysh-Parag-Paragogos-Def1','Basic','PdfLaTeX','Πρώτη παράγωγος',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def4','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Def4.tex','16/12/2021 23:08','-','@article{DTX-Statistikh-PaStD-BasEnnStat-Def4,}','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Def4
%@ Database source: Mathematics
Στατιστική είναι ένα σύνολο αρχών και μεθοδολογιών για:
\begin{itemize}
\item το σχεδιασμό της διαδικασίας συλλογής δεδομένων
\item τη συνοπτική και αποτελεσματική παρουσίασή τους
\item την ανάλυση και εξαγωγή αντίστοιχων συμπερασμάτων.
\end{itemize}
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Def4','Basic','PdfLaTeX','Στατιστική',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def5','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Def5.tex','16/12/2021 23:54','-','@article{DTX-Statistikh-PaStD-BasEnnStat-Def5,}','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Def5
%@ Database source: Mathematics
Πληθυσμός είναι ένα σύνολο του οποίου εξετάζουμε τα στοιχεία του ως προς ένα ή περισσότερα χαρακτηριστικά τους. Το πλήθος των στοιχείων ενός πληθυσμού ονομάζεται \textbf{μέγεθος} του πληθυσμού. Τα στοιχεία του πληθυσμού αναφέρονται και ως μονάδες ή άτομα του πληθυσμού.
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Def5','Basic','PdfLaTeX','Πληθυσμός',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def6','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Def6.tex','16/12/2021 23:56','-','@article{DTX-Statistikh-PaStD-BasEnnStat-Def6,}','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Def6
%@ Database source: Mathematics
Μεταβλητές λέγονται τα χαρακτηριστικά ως προς τα οποία εξετάζουμε έναν πληθυσμό και τις συμβολίζουμε συνήθως με κεφαλαία γράμματα $ X,Y,Z,A $...
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Def6','Basic','PdfLaTeX','Μεταβλητές - Στατιστική',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def7','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Def7.tex','17/12/2021 09:48','-','@article{DTX-Statistikh-PaStD-BasEnnStat-Def7,}','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Def7
%@ Database source: Mathematics
Τιμές μιας μεταβλητής $ X $ λέγονται οι δυνατές τιμές που μπορεί να πάρει μια μεταβλητή.
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Def7','Basic','PdfLaTeX','Τιμές μεταβλητής',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def8','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Def8.tex','17/12/2021 09:49','-','@article{DTX-Statistikh-PaStD-BasEnnStat-Def8,}','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Def8
%@ Database source: Mathematics
Από τη διαδοχική εξέταση των ατόμων του πληθυσμού ως προς ένα χαρακτηριστικό τους προκύπτει μια σειρά από δεδομένα, που λέγονται στατιστικά δεδομένα ή παρατηρήσεις. Συμβολίζονται με $ t_1,t_2,\ldots,t_{\nu} $.
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Def8','Basic','PdfLaTeX','Παρατηρήσεις',0,'Γ'' ΕΠΑΛ','','');
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def9','Def','Statistikh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Βασικές έννοιες στατιστικής - Παρουσίαση στατιστικών δεδομένων/Βασικές έννοιες στατιστικής/Ορισμοί/DTX-Statistikh-PaStD-BasEnnStat-Def9.tex','17/12/2021 09:58','-','@article{DTX-Statistikh-PaStD-BasEnnStat-Def9,}','%# Database File : DTX-Statistikh-PaStD-BasEnnStat-Def9
%@ Database source: Mathematics
Έστω $ X $ μια μεταβλητή που αφορά έναν πληθυσμό. Η μεταβλητή $ X $ λέγεται
\begin{itemize}
\item ποιοτική, αν οι τιμές της δεν είναι αριθμοί.
\item ποσοτική, αν οι τιμές της είναι αριθμοί. Οι ποσοτικές μεταβλητές χωρίζονται σε
\begin{itemize}
\item συνεχείς, αν μπορούν να πάρουν όλες τις τιμές μέσα σε ένα διάστημα $ (a,\beta) $ πραγματικών αριθμών.
\item διακριτές, αν μπορούν να πάρουν μεμονωμένες τις τιμές μέσα σε ένα διάστημα $ (a,\beta) $ πραγματικών αριθμών.
\end{itemize}
\end{itemize}
%# End of file DTX-Statistikh-PaStD-BasEnnStat-Def9','Basic','PdfLaTeX','Είδη μεταβλητών',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Γραμμική εξίσωση - Λύση -  Σημείο - Τομή με άξονες - Σχεδίαση/DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1.tex','05/1/2022 10:55','NO','@article{DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1,}','%# Database File : DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1
%@ Database source: Mathematics
Για καθεμιά από τις παρακάτω εξισώσεις να γραφτούν οι συντελεστές καθώς και ο σταθερός όρος.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ 2x+y=8 $
\item $ -3x+7y=-1 $
\item $ x=2 $
\item $ y=9 $
\item $ \sqrt{2}x+\frac{y}{4}=1 $
\item $ 2x=y $
\end{rlist}
\end{multicols}
%# End of file DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1','Basic','PdfLaTeX','Συντελεστές και σταθερός όρος εξίσωσης',0,'Β'' Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Γραμμική εξίσωση - Λύση -  Σημείο - Τομή με άξονες - Σχεδίαση/DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2.tex','05/1/2022 10:59','NO','@article{DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2,}','%# Database File : DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2
%@ Database source: Mathematics
Να σχεδιαστούν οι ακόλουθες ευθείες σε ορθογώνιο σύστημα συντεταγμένων.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ x-3y=6 $
\item $ 2x-y=-3 $
\item $ x=5 $
\item $ y=3 $
\end{rlist}
\end{multicols}
%# End of file DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2','Basic','PdfLaTeX','Χάραξη ευθείας',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Ορισμός συστήματος/DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1.tex','05/1/2022 12:31','NO','@article{DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1,}','%# Database File : DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1
%@ Database source: Mathematics
Για καθένα από τα παρακάτω γραμμικά συστήματα, να γράψετε τους συντελεστές των μεταβλητών και τους σταθερούς όρους.
\begin{rlist}[leftmargin=5mm]
\begin{multicols}{2}
\item $ \systeme{3x-y=5,2x+4y=8} $
\item $ \systeme{x-y=4,2x-5y=2} $
\item $ \systeme{\frac{3x}{4}-\frac{y}{2}=\frac{1}{5},\frac{x}{3}+\frac{3y}{4}=-\frac{3}{2}} $
\item $ \systeme{{0,1}x+{1.2}y=2,x-{0,4}y=3} $
\end{multicols}
\item $ \systeme{\sqrt{3}x-\sqrt{2}y=4,{\left(\sqrt{5}-1\right)}x-y=9} $
\end{rlist}
%# End of file DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1','Basic','PdfLaTeX','Εύρεση συντελεστών συστήματος',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Ορισμός συστήματος/DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2.tex','05/1/2022 12:32','NO','@article{DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2,}','%# Database File : DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2
%@ Database source: Mathematics
Για καθένα από τα παρακάτω γραμμικά συστήματα, να γράψετε τους συντελεστές των μεταβλητών και τους σταθερούς όρους.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{x-y=5,y=3} $
\item $ \systeme{x=2,y=-4} $
\item $ \systeme{x=2y,x-y=2} $
\item $ \systeme{x+3y=0,y=0} $
\end{rlist}
\end{multicols}
%# End of file DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2','Basic','PdfLaTeX','Εύρεση συντελεστών συστήματος',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντίθετων συντελεστών/DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1.tex','05/1/2022 12:54','NO','@article{DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1,}','%# Database File : DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1
%@ Database source: Mathematics
Να λυθούν τα παρακάτω συστήματα με τη μέθοδο των αντίθετων συντελεστών.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{2x+y=4,-4x-2y=3} $
\item $ \systeme{6x+3y=1,4x+2y=5} $
\item $ \ccases{\frac{x}{2}+\frac{y}{3}=2\\3x+2y=1} $
\end{rlist}
\end{multicols}
%# End of file DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1','Basic','PdfLaTeX','Μέθοδος αντίθετων συντελεστών - αδύνατο σύστημα',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος αντίθετων συντελεστών/DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2.tex','05/1/2022 13:16','NO','@article{DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2,}','%# Database File : DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2
%@ Database source: Mathematics
Να λυθούν τα παρακάτω γραμμικά συστήματα με τη μέθοδο των αντίθετων συντελεστών.
\begin{multicols}{2}
\begin{rlist}[leftmargin=5mm]
\item $ \systeme{x+2y=4,2x+4y=8} $
\item $ \systeme{12x+4y=8,-3x-y=2} $
\item $ \ccases{\frac{3x}{4}-\frac{y}{3}=2\\9x-4y=24} $
\end{rlist}
\end{multicols}
%# End of file DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2','Basic','PdfLaTeX','Μέθοδος αντίθετων συντελεστών - Αόριστο σύστημα',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Συστήματα εξισώσεων και ανισώσεων/Γραμμικά συστήματα/Ασκήσεις/Μέθοδος οριζουσών/DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1.tex','10/1/2022 16:07','NO','@article{DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1,}','%# Database File : DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1
%@ Database source: Mathematics
Να λυθούν τα παρακάτω συστήματα με τη μέθοδο των οριζουσών
\begin{multicols}{2}
\begin{alist}
\item $ \ccases{\left(\sqrt{2}+1\right)x+\sqrt{2}y=1-\sqrt{2}\\\sqrt{2}x+\left(\sqrt{2}-1\right)y=2-\sqrt{2}} $
\end{alist}
\end{multicols}
%# End of file DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-Def1','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ορισμοί/DTX-Algebra-AlgPar-Polyonyma-Def1.tex','13/1/2022 10:16','-','@article{DTX-Algebra-AlgPar-Polyonyma-Def1,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-Def1
%@ Database source: Mathematics
Μεταβλητή ονομάζεται το σύμβολο το οποίο χρησιμοποιούμε για εκφράσουμε έναν άγνωστο αριθμό. Συμβολίζεται με ένα γράμμα όπως $ x,y,z,a,\ldots $ κ.τ.λ.
%# End of file DTX-Algebra-AlgPar-Polyonyma-Def1','Basic','PdfLaTeX','Ορισμός μεταβλητής',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ασκήσεις/Ορισμός πολυωνύμου/DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1.tex','13/1/2022 10:22','NO','@article{DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1
%@ Database source: Mathematics
Για καθένα από τα παρακάτω πολυώνυμα να βρεθούν οι συντελεστές καθώς και ο σταθερός όρος.
\begin{multicols}{2}
\begin{alist}
\item $ P(x)=x^2-4x+3 $
\item $ P(x)=-2x^3+4x^2-7x+5 $
\item $ P(x)=x^4-x^2+5 $
\item $ P(x)=\dfrac{x^3}{4}-\dfrac{x^2}{2}+3x $
\item $ P(x)=\sqrt{2}x^3-(\sqrt{3}-1)x+2 $
\item $ P(x)=\dfrac{x^2-4x+4}{2} $
\end{alist}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1','Basic','PdfLaTeX','Εύρεση συντελεστών πολυωνύμου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ασκήσεις/Ορισμός πολυωνύμου/DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2.tex','13/1/2022 10:31','NO','@article{DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2
%@ Database source: Mathematics
Να βρεθούν οι όροι καθενός από τα παρακάτω πολυώνυμα.
\begin{multicols}{2}
\begin{alist}
\item $ P(x)=x^3-2x^2+5x-4 $
\item $ P(x)=2x^3-x+5 $
\item $ P(x)=3x-x^4+5+7x^2 $
\item $ P(x)=\sqrt{2}x^2+x-\sqrt{2}-1 $
\item $ P(x)=\dfrac{x^4}{3}-\dfrac{2x^2}{5}+1 $
\item $ P(x)=x^5-1 $
\end{alist}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2','Basic','PdfLaTeX','Εύρεση όρων πολυωνύμου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ασκήσεις/Βαθμός πολυωνύμου/DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1.tex','13/1/2022 10:33','NO','@article{DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1
%@ Database source: Mathematics
Να βρεθεί ο βαθμός καθενός από τα παρακάτω πολυώνυμα
\begin{multicols}{2}
\begin{alist}
\item $ P(x)=2x^3-4x^2+5x+7 $
\item $ P(x)=x^3-x^4+3x-2 $
\item $ P(x)=3x^2-2x+5x^3-8 $
\item $ P(x)=x^{12}-1 $
\end{alist}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1','Basic','PdfLaTeX','Εύρεση βαθμού πολυωνύμου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ασκήσεις/Τιμή - Ρίζα πολυωνύμου/DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1.tex','13/1/2022 12:09','NO','@article{DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1
%@ Database source: Mathematics
Δίνεται το πολυώνυμο $ P(x)=x^3-2x^2-5x+6 $. Να εξετάσετε αν οι αριθμοί $ -1,3,0,-2 $ αποτελούν ρίζες του $P(x)$.
%# End of file DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1','Basic','PdfLaTeX','Ρίζα πολυωνύμου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ασκήσεις/Βαθμός πολυωνύμου/DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2.tex','13/1/2022 12:21','NO','@article{DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2
%@ Database source: Mathematics
Δίνεται το πολυώνυμο $ P(x)=(\lambda^2-\lambda)x^2-(\lambda+1)x+2 $, όπου $ \lambda\in\mathbb{R} $. Να βρεθεί ο βαθμός του πολυωνύμου για κάθε τιμή της παραμέτρου $\lambda\in\mathbb{R}$.
%# End of file DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ασκήσεις/Πράξεις μεταξύ πολυωνύμων/DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1.tex','13/1/2022 12:32','NO','@article{DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1
%@ Database source: Mathematics
Δίνονται τα πολυώνυμα $ A(x)=x^2-3x+2 $ και $ B(x)=2x^2+5x-8 $. Να κάνετε τις ακόλουθες πράξεις:
\begin{multicols}{2}
\begin{alist}
\item $ A(x)+B(x) $
\item $ A(x)-B(x) $
\item $ B(x)-A(x) $
\item $ A(2x)+B(-x) $
\end{alist}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1','Basic','PdfLaTeX','Πρόσθεση πολυωνύμων',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντεταγμένες διαν. με γνωστά άκρα/DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1.tex','20/1/2022 14:00','YES','@article{DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1
%@ Database source: Mathematics
Δίνονται τα σημεία $ A(-3,2),B(4,1) $ και $ \vatGamma(0,7) $. Να βρεθούν οι συντεταγμένες των διανυσμάτων:
\begin{multicols}{3}
\begin{alist}
\item $ \overrightarrow{AB} $
\item $ \overrightarrow{A\varGamma} $
\item $ \overrightarrow{\varGamma B} $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντεταγμένες διαν. με γνωστά άκρα/DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2.tex','20/1/2022 15:31','YES','@article{DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2
%@ Database source: Mathematics
Δίνονται τα σημεία $ A(2,4) $ και $ B(-3,1) $.
\begin{alist}
\item Να βρεθούν οι συντεταγμένες του διανύσματος $ \overrightarrow{AB} $.
\item Να βρεθεί σημείο $ \varGamma $ τέτοιο ώστε $ \overrightarrow{A\varGamma}=(3,-3) $.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντεταγμένες διανύσματος/DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1.tex','20/1/2022 16:59','YES','@article{DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1
%@ Database source: Mathematics
Να βρεθούν οι συντεταγμένες των παρακάτω διανυσμάτων.
\begin{multicols}{3}
\begin{alist}
\item $ \vec{a}=3\vec{i}+2\vec{j} $
\item $ \vec{\beta}=-\vec{i}+4\vec{j} $
\item $ \vec{\gamma}=5\vec{i}-3\vec{j} $
\item $ \vec{\delta}=2\vec{j} $
\item $ \vec{x}=-7\vec{i} $
\item $ \vec{y}=\vec{j}-\vec{i} $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1','Basic','PdfLaTeX','Ορισμός διανύσματος',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντεταγμένες μέσου τμήματος - βαρύκεντρου/DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1.tex','20/1/2022 19:57','YES','@article{DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1
%@ Database source: Mathematics
Δίνονται τα σημεία $ A(2,-1) $ και $ B(-3,4) $.
\begin{alist}
\item Να βρεθούν οι συντεταγμένες του μέσου $ M $ του τμήματος $ AB $.
\item Να νρεθούν οι συντεταγμένες του σημείου $ \varGamma $ έτσι ώστε το $ B $ να είναι μέσο του τμήματος $ A\varGamma $.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1','Basic','PdfLaTeX','Συντεταγμένες μέσου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Ίσα διανύσματα - Μηδενικό δ. - Παράλληλα με άξονες/DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1.tex','20/1/2022 20:05','NO','@article{DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a}=(\lambda-2,\lambda) $ και $ \vec{\beta}=(-\lambda^2,2-\lambda) $ όπου $ \lambda\in\mathbb{R} $. Να βρεθούν οι τιμές τις παραμέτρου $ \lambda $ έτσι ώστε
\begin{alist}
\item τα διανύσματα $ \vec{a},\vec{\beta} $ να είναι ίσα.
\item τα $ \vec{a},\vec{\beta} $ να είναι αντίθετα.
\item $\vec{a}\parallel x''x$ και $ \vec{a}\neq\vec{0} $.
\item $ \vec{\beta}\parallel y''y $ και $ \vec{\beta}\neq\vec{0} $.
\item $ \vec{a}=\vec{0} $.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1','Basic','PdfLaTeX','Ίσα διανύσματα - Μηδενικό - Παράλληλα με άξονες',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Ίσα διανύσματα - Μηδενικό δ. - Παράλληλα με άξονες/DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2.tex','20/1/2022 20:10','NO','@article{DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2,}','%# Database File : DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a}=(\lambda^2-1,\lambda+1) $ και $ \vec{\beta}=(\lambda^2+\lambda,2+2\lambda) $ όπου $ \lambda\in\mathbb{R} $. Να βρεθούν οι τιμές τις παραμέτρου $ \lambda $ έτσι ώστε
\begin{alist}
\item τα διανύσματα $ \vec{a},\vec{\beta} $ να είναι ίσα.
\item $ \vec{a}=\vec{0} $.
\item $ \vec{\beta}=\vec{0} $.
\item $\vec{a}\parallel x''x$ και $ \vec{a}\neq\vec{0} $.
\item $ \vec{\beta}\parallel y''y $ και $ \vec{\beta}\neq\vec{0} $.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2','Basic','PdfLaTeX','Ίσα διανύσματα - Μηδενικό - Παράλληλα με άξονες',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Μέτρο διανύσματος/DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1.tex','20/1/2022 20:14','NO','@article{DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1
%@ Database source: Mathematics
Να βρεθεί το μέτρο καθενός από τα παρακάτω διανύσματα.
\begin{multicols}{2}
\begin{alist}
\item $ \vec{a}=(3,-4) $
\item $ \vec{a}=(12,5) $
\item $ \vec{a}=(2,0) $
\item $ \vec{a}=(0,7) $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1','Basic','PdfLaTeX','Μέτρο διανύσματος',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Μέτρο διανύσματος/DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2.tex','21/1/2022 18:05','NO','@article{DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2,}','%# Database File : DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2
%@ Database source: Mathematics
Δίνονται τα σημεία $ A(2,1),B(5,5) $ και $ \varGamma(-2,3) $.
Να βρεθούν τα μήκη των παρακάτω διανυσμάτων:
\begin{multicols}{3}
\begin{alist}
\item $\overrightarrow{AB}$ 
\item $\overrightarrow{A\varGamma} $
\item $\overrightarrow{B\varGamma}$
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2','Basic','PdfLaTeX','Μήκος διανύσματος με γνωστά άκρα',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντεταγμένες μέσου τμήματος - βαρύκεντρου/DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2.tex','22/1/2022 00:21','NO','@article{DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2
%@ Database source: Mathematics
Δίνεται παραλληλόγραμμο $ AB\varGamma\varDelta $ με $ A(4,1),B(-2,3) $ και $\varDelta(1,7)$.
\begin{alist}
\item Να βρεθεί το κέντρο $K$ του παραλληλογράμμου.
\item Να βρεθεί η κορυφή $ \varGamma $ του παραλληλογράμμου.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2','Basic','PdfLaTeX','Συντεταγμένες κέντρου - Γεωμετρική εφαρμογή',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντελεστής διεύθυνσης διανύσματος/DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1.tex','22/1/2022 00:27','NO','@article{DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1
%@ Database source: Mathematics
Για καθένα από τα παρακάτω διανύσματα να βρεθεί ο συντελεστής διεύθυνσης.
\begin{multicols}{2}
\begin{alist}
\item $ \vec{a}=(3,9) $
\item $ \vec{a}=(-4,2) $
\item $ \vec{a}=(-1,0) $
\item $ \vec{a}=(0,5) $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1','Basic','PdfLaTeX','Συντελεστής διεύθυνσης διανύσματος',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντεταγμένες γραμμικού συνδυασμού/DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1.tex','22/1/2022 00:33','NO','@article{DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a}=(2,1) $ και $ \vec{\beta}=(-3,4) $.
\begin{alist}
\item Να βρεθούν οι συντεταγμένες των διανυσμάτων $ \vec{u}=3\vec{a}-\vec{\beta} $ και $ \vec{v}=-2\vec{a}+5\vec{\beta} $.
\item Να γραφτεί το διάνυσμα $ \vec{\gamma}=(-10.17) $ ως γραμμικός συνδυασμός των $ \vec{a},\vec{\beta} $.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1','Basic','PdfLaTeX','Συντεταγμένες γραμμικού συνδυασμού',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Ασκήσεις/Μέτρο γραμμικού συνδυασμού/DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1.tex','22/1/2022 00:38','NO','@article{DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a},\vec{\beta} $ για τα οποία ισχύει $ |\vec{a}|=2,|\vec{\beta}|=4 $ και $ \hat{(\vec{a},\vec{\beta})}=60\degree $.
\begin{alist}
\item Να υπολογίσετε το εσωτερικό γινόμενο $ \vec{a}\cdot\vec{\beta} $.
\item Να βρεθεί το μέτρο του διανύσματος $\vec{u}=3\vec{a}-\vec{\beta}$.
\end{alist}
%# End of file DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συντελεστής διεύθυνσης διανύσματος/DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2.tex','22/1/2022 00:45','NO','@article{DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2
%@ Database source: Mathematics
Σε καθένα από τα παρακάτω ερωτήματα δίνονται τα άκρα $ A $ και $ B $ ενός διανύσματος $ \overrightarrow{AB} $. Να βρείτε, αν υπάρχει, το συντελεστή διεύθυνσης του διανύσματος.
\begin{multicols}{2}
\begin{alist}
\item $ A(1,2) $ και $ B(3,-2) $
\item $ A(4,1) $ και $ B(8,2) $
\item $ A(7,3) $ και $ B(5,3) $
\item $ A(2,1) $ και $ B(2,-3) $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2','Basic','PdfLaTeX','Συντελεστής διεύθυνσης διανύσματος',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Ασκήσεις/Συνημίτονο γωνίας διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1.tex','22/1/2022 01:07','NO','@article{DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1
%@ Database source: Mathematics
Δίνεται τρίγωνο $ AB\varGamma $ με κορυφές $ A(1,4),B(-1,0) $ και $ \varGamma(3,-2) $. Να υπολογίσετε τις γωνίες του τριγώνου.
%# End of file DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1','Basic','PdfLaTeX','Συνημίτονο γωνίας γιανυσμάτων - Γωνίες τριγώνου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Ασκήσεις/Εσωτερικό γινόμενο διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1.tex','22/1/2022 01:12','YES','@article{DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a}=(4,-2),\vec{\beta}=(-1,5) $ και $ \vec{\gamma}=(7,-4) $. Υπολογίστε τα παρακάτω εσωτερικά γινόμενα.
\begin{multicols}{3}
\begin{alist}
\item $ \vec{a}\cdot\vec{\beta} $
\item $ \vec{a}\cdot\vec{\gamma} $
\item $ \vec{\beta}\cdot\vec{\gamma} $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Ευθεία - Γωνίες/Εξίσωση ευθείας/Ασκήσεις/Εξίσωση ευθείας με γνωστό συντελεστή/DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1.tex','24/1/2022 09:39','YES','@article{DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1,}','%# Database File : DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1
%@ Database source: Mathematics
Σε καθένα από τα παρακάτω ερωτήματα να βρεθεί η εξίσωση της ευθείας που διέρχεται από το σημείο $ A(3,-4) $ και
\begin{alist}
\item έχει συντελεστή διεύθυνσης $ \lambda=2 $.
\item είναι παράλληλη με την ευθεία $ \varepsilon_1: y=3x-5 $.
\item είναι κάθετη στην ευθεία $ \varepsilon_2:y=\frac{1}{4}x+2 $.
\item είναι παράλληλη με τον άξονα $ x''x $.
\item σχηματίζει γωνία $ \omega=\frac{\pi}{4} $ με τον άξονα $ x''x $.
\item διέρχεται από το σημείο $ B(-1,2) $.
\item είναι κατακόρυφη.
\item είναι παράλληλη με το διάνυσμα $ \vec{a}=(-2,4) $.
\end{alist}
%# End of file DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1','Basic','PdfLaTeX','Ευθεία με γνωστό συντελεσή - Όλοι οι τρόποι εύρεσης συντελεστή διεύθυνσης',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Μέτρο διανύσματος/DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3.tex','24/1/2022 09:45','NO','@article{DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3,}','%# Database File : DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3
%@ Database source: Mathematics
Δίνεται τρίγωνο $ AB\varGamma $ με κορυφές $ A(-3,1),B(4,-2) $ και $ \varGamma(1,5) $. Να αποδείξετε ότι το τρίγωνο είναι ισοσκελές με βάση την πλευρά $ A\varGamma $.
%# End of file DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3','Basic','PdfLaTeX','Γεωμετρική εφαρμογή',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Ασκήσεις/Συνθήκη παραλληλίας διανυσμάτων/DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1.tex','24/1/2022 09:51','NO','@article{DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a}=(\lambda,\lambda-1) $ και $ \vec{\beta}=(\lambda+1,2) $. Να βρεθεί η τιμή της παραμέτρου $ \lambda\in\mathbb{R} $ ώστε $ \vec{a}\parallel\vec{\beta} $.
%# End of file DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Ασκήσεις/Εσωτερικό γινόμενο διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2.tex','24/1/2022 11:55','YES','@article{DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a},\vec{\beta} $ με $ |\vec{a}|=3,|\vec{\beta}|=4 $ και $\GwniaDianysmatwn{a}{\beta} =60\degree$. Να βρείτε το εσωτερικό γινόμενο $ \vec{a}\cdot\vec{\beta} $.
%# End of file DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2','Basic','PdfLaTeX','Απλή εφαρμογή του τύπου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Ασκήσεις/Εσωτερικό γινόμενο διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3.tex','24/1/2022 12:26','YES','@article{DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3
%@ Database source: Mathematics
Δίνονται τα σημεία $ Α(4,-1),Β(2,5) $ και $ \varGamma(-3,4) $. Να υπολογίσετε τα παρακάτω εσωτερικά γινόμενα:
\begin{multicols}{2}
\begin{alist}
\item $ \overrightarrow{AB}\cdot\overrightarrow{A\varGamma} $
\item $ \overrightarrow{A\varGamma}\cdot\overrightarrow{B\varGamma} $
\item $ \overrightarrow{AM}\cdot\overrightarrow{AB} $ όπου $ M $ είναι το μέσο του $ B\varGamma $.
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Ασκήσεις/Συνημίτονο γωνίας διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2.tex','24/1/2022 20:05','NO','@article{DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2
%@ Database source: Mathematics
Δίνονται τα διανύσματα $ \vec{a}=(2,3),\vec{\beta}=(4,2) $ και $ \vec{\gamma}=(-5,-3) $. Να υπολογίσετε τα συνημίτονα καθώς και τα είδη των γωνιών
\begin{multicols}{3}
\begin{alist}
\item $ \GwniaDianysmatwn{a}{\beta} $
\item $ \GwniaDianysmatwn{\beta}{\gamma} $
\item $ \GwniaDianysmatwn{a}{\gamma} $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Λύσεις ασκήσεων ενότητας/Συντεταγμένες διαν. με γνωστά άκρα/DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE1-1.tex','25/1/2022 00:48','-','@article{DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item $ \overrightarrow{AB}=(x_B-x_A,y_B-y_A)=(4-(-3),-1-2)=(7,-1) $
\item $ \overrightarrow{A\varGamma}=(x_{\varGamma}-x_A,y_{\varGamma}-y_A)=(0-(-3),7-2)=(3,5) $
\item $ \overrightarrow{\varGamma B}=(x_B-x_{\varGamma},y_B-y_{\varGamma})=(4-0,1-7)=(4,-6) $.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE2-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Λύσεις ασκήσεων ενότητας/Συντεταγμένες διαν. με γνωστά άκρα/DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE2-1.tex','25/1/2022 10:58','-','@article{DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE2-1
%@ Database source: Mathematics
\begin{alist}
\item $ \overrightarrow{AB}=(x_B-x_A,y_B-y_A)=(-3-2,1-4)=(-5,-3) $
\item Έστω $ \varGamma(x_{\varGamma},y_{\varGamma}) $ το ζητούμενο σημείο. Θα ισχύει
\begin{align*}
\overrightarrow{A\varGamma}&=(3,-3)\Rightarrow\\
(x_{\varGamma}-x_A,y_{\varGamma}-y_A)&=(3,-3)\Rightarrow\\
(x_{\varGamma}-2,y_{\varGamma}-4)&=(3,-3)
\end{align*}
άρα προκύπτει
\begin{itemize}
\item $ x_{\varGamma}-2=3\Rightarrow x_{\varGamma}=5 $
\item $ y_{\varGamma}-4=-3\Rightarrow y_{\varGamma}=1 $
\end{itemize}
Το σημείο θα είναι $ \varGamma(5,1) $
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE2-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Λύσεις ασκήσεων ενότητας/Συντεταγμένες διανύσματος/DTX-Geometria-Dian-SyntetDian-SyntDian-SolSE1-1.tex','25/1/2022 11:13','-','@article{DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntDian-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item $\vec{a}=3\vec{i}+2\vec{j}=(3,2)$
\item $\vec{\beta}=-\vec{i}+4\vec{j}=(-1,4)$
\item $\vec{\gamma}=5\vec{i}-3\vec{j}=(5,-3)$
\item $\vec{\delta}=2\vec{j}=0\vec{i}+2\vec{j}=(0,2)$
\item $\vec{x}=-7\vec{i}=-7\vec{i}+0\vec{j}=(-7,0)$
\item $\vec{y}=\vec{j}-\vec{i}=-1\vec{i}+1\vec{j}=(-1,1)$
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntDian-SolSE1-1','Basic','PdfLaTeX','Ορισμός διανύσματος',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Λύσεις ασκήσεων ενότητας/Εσωτερικό γινόμενο διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE1-1.tex','25/1/2022 12:35','-','@article{DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item $ \vec{a}\cdot\vec{\beta}=(4,-2)\cdot(-1,5)=4\cdot(-1)+(-2)\cdot 5=-4-10=-14 $
\item $ \vec{a}\cdot\vec{\gamma}=(4,-2)\cdot(7,-4)=4\cdot 7+(-2)\cdot(-4)=28+8=36 $
\item $ \vec{\beta}\cdot\vec{\gamma}=(-1,5)\cdot(7,-4)=(-1)\cdot 7+5\cdot(-4)=-7-20=-27 $
\end{alist}
%# End of file DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Συντεταγμένες διανύσματος/Λύσεις ασκήσεων ενότητας/Συντεταγμένες μέσου τμήματος - βαρύκεντρου/DTX-Geometria-Dian-SyntetDian-SyntMes-SolSE1-1.tex','25/1/2022 13:44','-','@article{DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1,}','%# Database File : DTX-Geometria-Dian-SyntetDian-SyntMes-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item Οι συντεταγμένες του $ M $ θα είναι
\[ x_M=\frac{x_A+x_B}{2}=\frac{2+(-3)}{2}=-\frac{1}{2} \]
και 
\[ y_M=\frac{y_A+y_B}{2}=\frac{-1+4}{2}=\frac{3}{2} \]
άρα $ M\left(-\frac{1}{2},\frac{3}{2}\right) $.
\item Το $ B $ είναι μέσο του $ A\varGamma $ οπότε
\[ x_B=\frac{x_A+x_{\varGamma}}{2}\Rightarrow -3=\frac{2+x_{\varGamma}}{2}\Rightarrow 2+x_{\varGamma}=-6\Rightarrow x_{\varGamma}=-8 \]
και 
\[ y_B=\frac{y_A+y_{\varGamma}}{2}\Rightarrow 4=\frac{-1+y_{\varGamma}}{2}\Rightarrow -1+y_{\varGamma}=8\Rightarrow y_{\varGamma}=9 \]
Επομένως $ \varGamma(-8,9) $.
\end{alist}
%# End of file DTX-Geometria-Dian-SyntetDian-SyntMes-SolSE1-1','Basic','PdfLaTeX','Συντεταγμένες μέσου',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1','SectEx','Statistikh',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Στατιστική/Μέτρα θέσης και διασποράς/Μέτρα διασποράς/Ασκήσεις/Μέτρα διασποράς από παρατηρήσεις/DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1.tex','26/1/2022 17:14','NO','@article{DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1,}','%# Database File : DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1
%@ Database source: Mathematics
Εξετάσαμε ως προς την τιμή του ψωμιού, 10 φούρνους σε μια πόλη και πήραμε τα παρακάτω αποτελέσματα:
\begin{center}
\begin{tabular}{ccccc}
1.1 & 1 & 0.8 & 1.2 & 1\\
0.9 & 1.1 & 1 & 0.9 & 1
\end{tabular}
\end{center}
Να υπολογίσετε
\begin{alist}
\item τη μέση τιμή του δείγματος.
\item τη διακύμανση και την τυπική απόκλιση.
\end{alist}
%# End of file DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1','Basic','PdfLaTeX','Μέση τιμή, Διασπορά και απόκλιση. Μικρό δείγμα παρατηρήσεων',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1.tex','27/1/2022 10:11','YES','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1
%@ Database source: Mathematics
Σε καθένα από τα παρακάτω ερωτήματα να βρείτε την εξίσωση του κύκλου:
\begin{alist}
\item με κέντρο $ K(-3,2) $ και ακτίνα $ \rho=4 $.
\item με διάμετρο $ AB $ όπου $ A(1,0) $ και $ B(3,4) $.
\item με κέντρο $ Κ(2,4) $ και διέρχεται από το σημείο $ A(5,0) $.
\item με κέντρο $ K(-1,2) $ και εφαπτομένη την ευθεία $ \varepsilon: x+2y-2=0 $.
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1','Basic','PdfLaTeX','Εξίσωση κύκλου με διάφορους τρόπους να βρεθούν κέντρο και ακτίνα',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Λύσεις ασκήσεων ενότητας/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE1-1.tex','27/1/2022 10:16','-','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item Ο κύκλος με κέντρο $ K(-3,2) $ και ακτίνα $ \rho=4 $ έχει εξίσωση
\[ (x-x_K)^2+(y-y_K)^2=\rho^2\Rightarrow(x-(-3))^2+(y-2)=4^2\Rightarrow (x+3)^2+(y-2)^2=16 \]
\item Το κέντρο $ K $ του κύκλου είναι το μέσο της διαμέτρου $ AB $ άρα
\[ x_K=\frac{x_A+x_B}{2}=\frac{1+3}{2}=2\ \text{ και }\ y_K=\frac{y_A+y_B}{2}=\frac{0+4}{2}=2 \]
οπότε $ K(2,2) $. Επιπλέον η ακτίνα ισούται με 
\[ \rho=KA=\sqrt{(x_A-x_K)^2+(y_A-y_K)^2}=\sqrt{(1-2)^2+(0-2)^2}=\sqrt{5} \]
Ο κύκλος λοιπόν θα έχει εξίσωση
\[ (x-2)^2+(y-2)^2=\sqrt{5}^2\Rightarrow (x-2)^2+(y-2)^2=5 \]
\item Αφού ο κύκλος διέρχεται από το σημείο $ A(5,0) $ τότε η ακτίνα του θα ισούται με
\[ \rho=KA=\sqrt{(x_A-x_K)^2+(y_A-y_K)^2}=\sqrt{(5-2)^2+(0-4)^2}=\sqrt{9+16}=5 \]
οπότε ο κύκλος έχει εξίσωση
\[ (x-2)^2+(y-4)^2=5^2\Rightarrow (x-2)^2+(y-4)^2=25 \]
\item Η ακτίνα του κύκλου ισούται με την απόσταση του κέντρου $ K $ από την εφαπτομένη $ \varepsilon $.
\[ \rho=d(K,\varepsilon)=\frac{|1\cdot(-1)+2\cdot 2-2|}{\sqrt{1^2+2^2}}=\frac{1}{\sqrt{5}}=\frac{\sqrt{5}}{5} \]
άρα η ζητούμενη εξίσωση είναι
\[ (x-(-1))^2+(y-2)^2=\left(\frac{\sqrt{5}}{5}\right)^2\Rightarrow (x+1)^2+(y-2)^2=\frac{1}{5} \]
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE1-1','Basic','PdfLaTeX','Εξίσωση κύκλου με διάφορους τρόπους να βρεθούν κέντρο και ακτίνα',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE2-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Λύσεις ασκήσεων ενότητας/Εσωτερικό γινόμενο διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE2-1.tex','27/1/2022 17:08','-','@article{DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE2-1
%@ Database source: Mathematics
\[ \vec{a}\cdot\vec{\beta}=|\vec{a}|\cdot|\vec{\beta}|\cdot\syn{\GwniaDianysmatwn{a}{\beta}}=3\cdot 4\cdot\syn{60\degree}=12\cdot\frac{1}{2}=6 \]
%# End of file DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE2-1','Basic','PdfLaTeX','Απλή εφαρμογή του τύπου',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE3-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Διανύσματα/Εσωτερικό γινόμενο/Λύσεις ασκήσεων ενότητας/Εσωτερικό γινόμενο διανυσμάτων/DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE3-1.tex','27/1/2022 19:03','-','@article{DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3,}','%# Database File : DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE3-1
%@ Database source: Mathematics
\begin{alist}
\item $ \overrightarrow{AB}=(2-4,5-(-1))=(-2,6) $ και $ \overrightarrow{A\varGamma}=(-3-4,4-(-1))=(-7,5) $. Άρα
\[ \overrightarrow{AB}\cdot\overrightarrow{A\varGamma}=(-2,6)\cdot(-7,5)=(-2)\cdot(-7)+6\cdot 5=14+30=44 \]
\item $ \overrightarrow{B\varGamma}=(-3-2,4-5)=(-5,-1) $ οπότε
\[ \overrightarrow{A\varGamma}\cdot\overrightarrow{B\varGamma}=(-7,5)\cdot(-5,-1)=(-7)\cdot(-5)+5\cdot(-1)=35-5=30 \]
Το $ M $ είναι μέσο του $ B\varGamma $ οπότε 
\[ x_M=\frac{x_B+x_{\varGamma}}{2}=\frac{2-3}{2}=-\frac{1}{2} \]
και
\[ y_M=\frac{y_B+y_{\varGamma}}{2}=\frac{5+4}{2}=\frac{9}{2} \]
οπότε $ M\left(-\frac{1}{2},\frac{9}{2}\right) $.
\end{alist}
%# End of file DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE3-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Ευθεία - Γωνίες/Εξίσωση ευθείας/Λύσεις ασκήσεων ενότητας/Εξίσωση ευθείας με γνωστό συντελεστή/DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SolSE1-1.tex','28/1/2022 17:19','-','@article{DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1,}','%# Database File : DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item Η εξίσωση της ευθείας είναι
\begin{align*}
y-y_A&=\lambda(x-x_A)\Rightarrow\\
y-(-4)&=2(x-3)\Rightarrow\\
y+4&=2x-6\Rightarrow\\
y&=2x-10
\end{align*}
\item Αφού ισχύει $ \varepsilon\parallel\varepsilon_1\Rightarrow \lambda_{\varepsilon}=\lambda_{\varepsilon_1}\Rightarrow \lambda_{\varepsilon}=3 $. Οπότε η εξίσωση της ευθείας είναι:
\begin{align*}
y-y_A&=\lambda(x-x_A)\Rightarrow\\
y-(-4)&=3(x-3)\Rightarrow\\
y+4&=3x-9\Rightarrow\\
y&=3x-13
\end{align*}
\item Έχουμε ότι $ \varepsilon\perp\varepsilon_1\Rightarrow \lambda_{\varepsilon}\cdot\lambda_{\varepsilon_1}=-1\Rightarrow\lambda_{\varepsilon}\cdot\frac{1}{4}=-1\Rightarrow \lambda_{\varepsilon}=-4 $. Άρα
\begin{align*}
y-y_A&=\lambda(x-x_A)\Rightarrow\\
y-(-4)&=-4(x-3)\Rightarrow\\
y+4&=-4x+12\Rightarrow\\
y&=-4x+8
\end{align*}
\item Αφού η ευθεία είναι οριζόντια τότε $ \lambda=0 $ άρα έχει εξίσωση
\[ y-y_A=\lambda(x-x_A)\Rightarrow y-(-4)=0(x-3)\Rightarrow y=-4 \]
\end{alist}
%# End of file DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SolSE1-1','Basic','PdfLaTeX','Ευθεία με γνωστό συντελεσή - Όλοι οι τρόποι εύρεσης συντελεστή διεύθυνσης',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Έλλειψη - Εξίσωση έλλειψης/Ασκήσεις/Στοιχεία έλλειψης (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)/DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1.tex','02/2/2022 01:01','YES','@article{DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1,}','%# Database File : DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1
%@ Database source: Mathematics
Δίνεται η έλλειψη $ C $ με εξίσωση
\[ \frac{x^2}{25}+\frac{y^2}{16}=1 \]
Να βρεθούν
\begin{alist}
\item οι εστίες της έλλειψης.
\item οι άξονες και οι κορυφές της.
\item η εκκεντρότητα.
\end{alist}
%# End of file DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1','Basic','PdfLaTeX','Στοιχεία έλλειψης από εξίσωση',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Έλλειψη - Εξίσωση έλλειψης/Λύσεις ασκήσεων ενότητας/Στοιχεία έλλειψης (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)/DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE1-1.tex','02/2/2022 01:01','-','@article{DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1,}','%# Database File : DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item Σύμφωνα με την εξίσωση της έλλειψης, παρατηρούμε ότι $ a^2=25\Rightarrow a=5 $ και $\beta^2=16\Rightarrow \beta=4$. Επομένως οι εστίες βρίσκονται στον άξονα $ x''x $. Έχουμε λοιπόν
\[ \beta^2=a^2-\gamma^2\Rightarrow \gamma^2=a^2-\gamma^2=25-16=9\Rightarrow \gamma=3 \]
Άρα οι εστίες είναι $ E(3,0) $ και $ E''(-3,0) $.
\item Ο μεγάλος άξονας έχει μήκος $ AA''=2a=2\cdot 5=10 $ και ο μικρός $ BB''=2\beta=2\cdot 4=8 $. 
\item Η εκκεντρότητα ισούται με 
\[ \varepsilon=\frac{\gamma}{a}=\frac{3}{5} \]
\end{alist}
%# End of file DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE1-1','Basic','PdfLaTeX','Στοιχεία έλλειψης από εξίσωση',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Έλλειψη - Εξίσωση έλλειψης/Ασκήσεις/Στοιχεία έλλειψης (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)/DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2.tex','02/2/2022 10:19','YES','@article{DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2,}','%# Database File : DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2
%@ Database source: Mathematics
Να βρεθεί η εξίσωση της έλλειψης
\begin{alist}
\item με εστίες $ E(3,0) $ και $ E''(-3,0) $ και εκκεντρότητα $\varepsilon=0{,}6$.
\item η οποία έχει μήκη αξόνων $ A''A=8 $ και $ B''B=6 $.
\item με εκκεντρότητα $\varepsilon=0{,}8$ και μήκος μεγάλου άξονα $ A''A=10 $.
\item με εκκεντρότητα $ \varepsilon=\frac{\sqrt{3}}{2} $ και μήκος μικρού άξονα $ B''B=4 $.
\item με κορυφές $ A(7,0),A''(-7,0),B(0,4) $ και $ Β''(0,-4) $.
\end{alist}
%# End of file DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2','Basic','PdfLaTeX','Εξίσωση έλλειψης από στοιχεία της',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE2-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Έλλειψη - Εξίσωση έλλειψης/Λύσεις ασκήσεων ενότητας/Στοιχεία έλλειψης (Εξίσωση-εστίες-παράμετροι-άξονες-κορυφές)/DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE2-1.tex','02/2/2022 23:46','-','@article{DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2,}','%# Database File : DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE2-1
%@ Database source: Mathematics
\begin{alist}
\item Οι εστίες βρίσκονται πάνω στον άξονα $ x''x $ άρα η εξίσωση της έλλειψης έχει τη μορφή
\[ \frac{x^2}{a^2}+\frac{y^2}{\beta^2}=1 \]
Από τις συντεταγμένες των εστιών προκύπτει $\gamma=3$. Επιπλέον έχουμε
\[ \varepsilon=0{,}6\Rightarrow\frac{\gamma}{a}=\frac{6}{10}\Rightarrow \frac{3}{a}=\frac{3}{5}\Rightarrow a=5 \]
Άρα παίρνουμε
\[ \beta^2=a^2-\gamma^2=5^2-3^2=25-9=16 \]
Η εξίσωση της έλλειψης θα είναι
\[ \frac{x^2}{25}+\frac{y^2}{16}=1 \]
\item Παρατηρούμε ότι ο μεγάλος άξονας της έλειψης είναι ο οριζόντιος επομένως $ 2a=8\Rightarrow a=4 $ και $ 2\beta=6\Rightarrow\beta=3 $. Άρα η εξίσωση της έλλειψης θα είναι 
\[ \frac{x^2}{a^2}+\frac{y^2}{\beta^2}=1\Rightarrow\frac{x^2}{16}+\frac{y^2}{9}=1 \]
\item Από την εκκεντρότητα της έλλειψης παίρνουμε
\[ \varepsilon=0{,}8\Rightarrow \frac{\gamma}{a}=0{,}8\Rightarrow \gamma=0{,}8a \]
Επιπλέον ο μεγάλος άξονας είναι ο οριζόντιος. Θα είναι
\[ A''A=10\Rightarrow 2a=10\Rightarrow a=5 \]
Άρα $ \gamma=0{,}8a=0{,}8\cdot 5=4 $. Έχουμε λοιπόν
\begin{align}
\beta^2=a^2-\gamma^2&\Rightarrow \beta^2=5^2-4^4\\&\Rightarrow \beta^2=9
\end{align}
Επομένως η εξίσωση της έλλειψης θα είναι
\[ \frac{x^2}{a^2}+\frac{y^2}{\beta^2}=1\Rightarrow \frac{x^2}{25}+\frac{y^2}{9}=1 \]
\item Έχουμε $ \varepsilon=\frac{\sqrt{3}}{2}\Rightarrow \frac{\gamma}{a}=\frac{\sqrt{3}}{2}\Rightarrow \gamma =\frac{\sqrt{3}}{2} a $. Επίσης Από το μήκος του μικρού άξονα προκύπτει
\[ B''B=4\Rightarrow 2\beta=4\Rightarrow \beta=2 \]
Άρα
\[ \beta^2=a^2-\gamma^2\Rightarrow 2^2=a^2-\left(\frac{\sqrt{3}}{2}a\right)^2\Rightarrow \frac{1}{4}a^2=4\Rightarrow a^2=16 \]
Η εξίσωση της έλλειψης θα είναι
\[ \frac{x^2}{a^2}+\frac{y^2}{\beta^2}=1\Rightarrow\frac{x^2}{16}+\frac{y^2}{4}=1 \]
\item Απο τις συντεταγμένες των κορυφών συμπαιρένουμε ότι ο μεγάλος άξονας είναι ο οριζόντιος οπότε η εξίσωση της έλλειψης έχει τη μορφή
\[ \frac{x^2}{a^2}+\frac{y^2}{\beta^2}=1 \]
Επιπλέον από τα σημεία αυτά έχουμε άμεσα ότι $a=7$ και $\beta=4$ άρα
\[ \frac{x^2}{7^2}+\frac{y^2}{4^2}=1\Rightarrow \frac{x^2}{49}+\frac{y^2}{16}=1 \]
\end{alist}
%# End of file DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE2-1','Basic','PdfLaTeX','Εξίσωση έλλειψης από στοιχεία της',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2.tex','04/2/2022 13:48','YES','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2
%@ Database source: Mathematics
Να βρεθούν τα κέντρα και οι ακτίνες των παρακάτω κύκλων
\begin{alist}
\item $ (x-2)^2+(y-3)^2=25 $
\item $ (x+1)^2+(y-2)^2=16 $
\item $ x^2+(y+4)^2=10 $
\item $ (x-3)^2+y^2=1 $
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2','Basic','PdfLaTeX','Κέντρο και ακτίνα από εξίσωση κύκλου',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE2-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Λύσεις ασκήσεων ενότητας/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE2-1.tex','04/2/2022 13:48','-','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE2-1
%@ Database source: Mathematics
\begin{alist}
\item $ (x-2)^2+(y-3)^2=25\Rightarrow (x-2)^2+(y-3)^2=5^2 $ οπότε το κέντρο είναι $ K(2,3) $ και η ακτίνα $ \rho=5 $.
\item $ (x+1)^2+(y-2)^2=16\Rightarrow (x-(-1))^2+(y-2)^2=4^2 $ άρα $ K(-1,2) $ και $ \rho=4 $.
\item $ x^2+(y+4)^2=10\Rightarrow (x-0)^2+(y-(-4))^2=\sqrt{10}^2 $ άρα $ K(0,-4) $ και $ \rho=\sqrt{10} $.
\item $ (x-3)^2+y^2=1\Rightarrow (x-3)^2+(y-0)^2=1^2 $ άρα $ K(3,0) $ και $ \rho=1 $.
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE2-1','Basic','PdfLaTeX','Κέντρο και ακτίνα από εξίσωση κύκλου',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3.tex','04/2/2022 13:59','YES','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3
%@ Database source: Mathematics
Να βρεθεί η εξίσωση του κύκλου με κέντρο την αρχή των αξόνων $ O(0,0) $ 
\begin{alist}
\item και ακτίνα $ \rho=4 $.
\item ο οποίος διέρχεται από το σημείο $ A(4,2) $.
\item ο οποίος εφάπτεται στην ευθεία $ \varepsilon : 3x+4y-2=0 $.
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3','Basic','PdfLaTeX','Εξίσωση κύκλου από στοιχεία.',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE3-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Λύσεις ασκήσεων ενότητας/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE3-1.tex','04/2/2022 13:59','-','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE3-1
%@ Database source: Mathematics
Σε καθένα από τα παρακάτω ερωτήματα ο κύκλος έχει εξίσωση της μορφής $ x^2+y^2=\rho^2 $.
\begin{alist}
\item Με ακτίνα $ \rho=4 $ εξίσωση του κύκλου θα είναι
\[ x^2+y^2=\rho^2\Rightarrow x^2+y^2=4^2\Rightarrow x^2+y^2=16 \]
\item Η ακτίνα του κύκλου ισούται με
\[ OA=\sqrt{4^2+2^2}=\sqrt{16+4}=\sqrt{20} \]
οπότε η εξίσωση είναι
\[ x^2+y^2=\rho^2\Rightarrow x^2+y^2=\sqrt{20}\Rightarrow x^2+y^2=20 \]
\item Η ακτίνα του κύκλου ισούται με την απόσταση του κέντρου από την ακτινα:
\[ \rho=d(O,\varepsilon)=\frac{|3\cdot 0+4\cdot 0-2|}{\sqrt{3^2+4^2}}=\frac{|-2|}{\sqrt{25}}=\frac{2}{5} \]
οπότε έχουμε
\[ x^2+y^2=\left(\frac{2}{5}\right)^2\Rightarrow x^2+y^2=\frac{4}{25} \]
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE3-1','Basic','PdfLaTeX','Εξίσωση κύκλου από στοιχεία.',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4.tex','04/2/2022 17:42','YES','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4
%@ Database source: Mathematics
Σε καθένα από τα παρακάτω ερωτήματα να βρεθεί η εξίσωση του κύκλου με κέντρο $ K $ και ακτίνα $ \rho $.
\begin{multicols}{2}
\begin{alist}
\item $ Κ(2,-3) $ και $ \rho=4 $.
\item $ Κ(-4,5) $ και $ \rho=3 $.
\item $ K(1,1) $ και $ \rho=\sqrt{5} $
\item $ K(0,-2) $ και $ \rho=\frac{1}{2} $
\item $ K(7,0) $ και $\rho=\frac{\sqrt{3}}{3}$.
\item $ Κ\left(\frac{3}{2},-\frac{1}{2}\right) $ και $ \rho=1 $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4','Basic','PdfLaTeX','Εξίσωση κύκλου με κέντρο και ακτίνα',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE4-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Λύσεις ασκήσεων ενότητας/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE4-1.tex','07/2/2022 23:55','-','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE4-1
%@ Database source: Mathematics
\begin{alist}
\item Με κέντρο $ K(2,-3) $ και ακτίνα $ \rho=4 $ ο κύκλος έχει εξίσωση
\[ (x-x_K)^2+(y-y_K)^2=\rho^2\Rightarrow (x-2)^2+(y-(-3))^2=4^2\Rightarrow (x-2)^2+(y+3)^2=16 \]
\item $ (x-x_K)^2+(y-y_K)^2=\rho^2\Rightarrow (x-(-4))^2+(y-5)^2=3^2\Rightarrow (x+4)^2+(y-5)^2=9 $
\item $ (x-x_K)^2+(y-y_K)^2=\rho^2\Rightarrow (x-1)^2+(y-1)^2=\sqrt{5}^2\Rightarrow (x-1)^2+(y-1)^2=5 $
\item $ (x-x_K)^2+(y-y_K)^2=\rho^2\Rightarrow (x-0)^2+(y-(-2))^2=\left(\frac{1}{2}\right)^2\Rightarrow x^2+(y+2)^2=\frac{1}{4} $
\item $ (x-x_K)^2+(y-y_K)^2=\rho^2\Rightarrow (x-7)^2+(y-0)^2=\left(\frac{\sqrt{3}}{3}\right)^2\Rightarrow (x-7)^2+y^2=\frac{1}{3} $
\item $ (x-x_K)^2+(y-y_K)^2=\rho^2\Rightarrow \left(x-\frac{3}{2}\right)^2+\left(y-\frac{1}{2}\right)^2=1^2\Rightarrow \left(x-\frac{3}{2}\right)^2+\left(y-\frac{1}{2}\right)^2=1 $
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE4-1','Basic','PdfLaTeX','Εξίσωση κύκλου με κέντρο και ακτίνα',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Arith-Arithmoi-Def1','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αριθμοί/Αριθμοί/Ορισμοί/DTX-Algebra-Arith-Arithmoi-Def1.tex','10/2/2022 16:35','-','@article{DTX-Algebra-Arith-Arithmoi-Def1,}','%# Database File : DTX-Algebra-Arith-Arithmoi-Def1
%@ Database source: Mathematics
Φυσικοί ονομάζονται οι αριθμοί
\[ 0,1,2,3,4,5,\ldots \]
\begin{itemize}
\item Κάθε αριθμός έχει διαφορά μιας μονάδας από τον προηγούμενο και τον επόμενό του.
\item Ο αριθμός $ 0 $ δεν έχει προηγούμενο.
\end{itemize}
%# End of file DTX-Algebra-Arith-Arithmoi-Def1','Basic','PdfLaTeX','',0,'Α'' Γυμνασίου','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Arith-Arithmoi-Def2','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αριθμοί/Αριθμοί/Ορισμοί/DTX-Algebra-Arith-Arithmoi-Def2.tex','10/2/2022 16:39','-','@article{DTX-Algebra-Arith-Arithmoi-Def2,}','%# Database File : DTX-Algebra-Arith-Arithmoi-Def2
%@ Database source: Mathematics
Άρτιοι ή αλλιώς ζυγοί ονομάζονται οι φυσικοί αριθμοί που διαιρούνται με το $ 2 $ ενώ περιττοί ή μονοί λέγονται αυτοί που δεν διαιρούνται με το $ 2 $. 
\begin{itemize}
\item Το τελευταίο ψηφίο κάθε άρτιου αριθμού είναι ένα από τα $ 0,2,4,6,8 $. 
\item Ένας περιττός αριθμός τελείώνει σε ένα από τα ψηφία $ 1,3,5,7,9 $.
\end{itemize}
%# End of file DTX-Algebra-Arith-Arithmoi-Def2','Basic','PdfLaTeX','Άρτιοι - Περιττοί αριθμοί (Απλή μορφή)',0,'Α'' Γυμνασίου','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Arith-Arithmoi-Tab1','Tab','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αριθμοί/Αριθμοί/ΠίνακεςDTX-Algebra-Arith-Arithmoi-Tab1.tex','10/2/2022 16:48','-','@article{DTX-Algebra-Arith-Arithmoi-Tab1,}','%# Database File : DTX-Algebra-Arith-Arithmoi-Tab1
%@ Database source: Mathematics
\begin{longtable}{c|>{\centering\arraybackslash}m{1.5cm}|>{\centering\arraybackslash}m{1.7cm}|>{\centering\arraybackslash}m{1.4cm}|>{\centering\arraybackslash}m{1cm}|>{\centering\arraybackslash}m{1.5cm}|>{\centering\arraybackslash}m{1.2cm}|>{\centering\arraybackslash}m{1cm}}
\hline  \multicolumn{8}{c}{\textbf{Ψηφία Ακέραιου Αριθμού}} \rule[-2ex]{0pt}{5.5ex}\\ 
\hhline{========} \rule[-2ex]{0pt}{6ex} \begin{minipage}{1.5cm}
\begin{center}
{\footnotesize \textbf{Δεκαδική}}\\{\footnotesize \textbf{Τάξη}}
\end{center}
\end{minipage} &
{\footnotesize Εκατομμύρια} & 
\begin{minipage}{1.5cm}
\begin{center}
\vspace{-1.9mm}
{\footnotesize Εκατοντάδες}\\}{\footnotesize Χιλιάδες}
\end{center}
\end{minipage} & 
\begin{minipage}{1.3cm}
\begin{center}
{\footnotesize Δεκάδες}\\{\footnotesize Χιλιάδες}
\end{center}
\end{minipage} & 
{\footnotesize Χιλιάδες}& 
{\footnotesize Εκατοντάδες} & 
{\footnotesize Δεκάδες}& 
{\footnotesize Μονάδες}  \\ 
\hline \rule[-1.5ex]{0pt}{4.5ex} {\footnotesize \textbf{Συμβ.}} & {\footnotesize Εκ} & {\footnotesize ΕΧ} & {\footnotesize ΔΧ} & {\footnotesize Χ} & {\footnotesize Ε} & {\footnotesize Δ} & {\footnotesize Μ} \\ 
\hline \rule[-1.5ex]{0pt}{4.5ex} {\footnotesize \textbf{Αξία}} & {\footnotesize $ 1.000.000 $} & {\footnotesize $ 100.000 $} & {\footnotesize $ 10.000 $} & {\footnotesize $ 1.000 $} & {\footnotesize $ 100 $} & {\footnotesize $ 10 $} & {\footnotesize $ 1 $}\\
\hline 
\end{longtable}
%# End of file DTX-Algebra-Arith-Arithmoi-Tab1','Basic','PdfLaTeX','Δεκαδικές αξίες',0,'Α'' Γυμνασίου','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-Arith-Arithmoi-Def3','Def','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αριθμοί/Αριθμοί/Ορισμοί/DTX-Algebra-Arith-Arithmoi-Def3.tex','10/2/2022 16:47','-','@article{DTX-Algebra-Arith-Arithmoi-Def3,}','%# Database File : DTX-Algebra-Arith-Arithmoi-Def3
%@ Database source: Mathematics
Ο άξονας των φυσικών αριθμών είναι μια αριθμημένη ημιευθεία στην οποία μπορούν να τοποθετηθούν όλοι οι φυσικοί αριθμοί σε αύξουσα σειρά από τα αριστερά προς τα δεξιά.
\begin{center}
\begin{tikzpicture}
\tkzInit[xmin=0,xmax=4]
\draw[-latex] (0,0) -- coordinate (x axis mid) (4.4,0) node[right,fill=white] {{\footnotesize $ x $}};
\foreach \x in {0,...,4}
\draw (\x,.5mm) -- (\x,-.5mm) node[anchor=north,fill=white] {{\scriptsize \x}};
\tkzText(2,0.7){Φυσικοί Αριθμοί}
\tkzDefPoint(2,0){A}
\tkzDrawPoint(A)
\tkzLabelPoint[above](A){{\scriptsize $ A(2) $}}
\end{tikzpicture}
\end{center}
\begin{itemize}[itemsep=0mm]
\item \textbf{Αρχή} του άξονα είναι το σημείο στο οποίο βρίσκεται ο αριθμός $ 0 $.
\item Η θέση ενός αριθμού πάνω στην ευθεία σχεδιάζεται με ένα σημείο.
\item Ο αριθμός που βρίσκεται στη θέση αυτή ονομάζεται \textbf{τετμημένη} του σημείου.
\end{itemize}
%# End of file DTX-Algebra-Arith-Arithmoi-Def3','Basic','PdfLaTeX','',0,'Α'' Γυμνασίου','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Έλλειψη - Εξίσωση έλλειψης/Ασκήσεις/Εξίσωση εφαπτομένης/DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1.tex','11/2/2022 17:17','NO','@article{DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1,}','%# Database File : DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1
%@ Database source: Mathematics
Δίνεται η έλλειψη $ C $ με εξίσωση
\[ \frac{x^2}{12}+\frac{y^2}{4}=1 \]
Να βρεθεί η εξίσωση της εφαπτομένης $ \varepsilon $ της έλλειψης
\begin{alist}
\item στο σημείο $ A(3,1) $
\item η οποία είναι παράλληλη με την ευθεία $ \varepsilon_1:y=x-1 $
\end{alist}
%# End of file DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1','Basic','PdfLaTeX','Εξίσωση εφαπτομένης - Διάφορα ερωτήματα',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Γενική εξίσωση κύκλου/DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1.tex','11/2/2022 17:26','YES','@article{DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1
%@ Database source: Mathematics
Να βρεθεί τι σχήμα παριστάνει κάθεμία από τις παρακάτω εξισώσεις
\begin{alist}
\item $ x^2+y^2+2x+4y-4=0 $
\item $ x^2+y^2+6x-10y+34=0 $
\item $ x^2+y^2-2x+4y+9=0 $
\item $ x^2+y^2+x+2y+\frac{1}{4}=0 $
\item $ 4x^2+4y^2+12x+16y-20=0 $
\item $ x^2+y^2+8y+20=0 $
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SolSE1-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Λύσεις ασκήσεων ενότητας/Γενική εξίσωση κύκλου/DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SolSE1-1.tex','11/2/2022 17:26','-','@article{DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item Για την εξίσωση $ x^2+y^2+2x+4y-4=0 $ έχουμε $ A=2,B=4 $ και $ \varGamma=-4 $. Είναι λοιπόν
\[ A^2+B^2-4\varGamma=2^2+4^2-4\cdot(-4)=4+16+16=36>0 \]
Η εξίσωση παριστάνει κύκλο με ακτίνα
\[ \rho=\frac{\sqrt{A^2+B^2-4\varGamma}}{2}=\frac{\sqrt{36}}{2}=\frac{6}{2}=3 \]
και κέντρο
\[ K\left(-\frac{A}{2},-\frac{B}{2}\right)\equiv K\left(-\frac{2}{2},-\frac{4}{2}\right)\equiv K(-1,-2) \]
\item Έχουμε ότι $ A=6,B=-10 $ και $ \varGamma=34 $.
\[ A^2+B^2+4\varGamma=6^2+(-10)^2-4\cdot 34=36+100-136=0 \]
Η εξίσωση λοιπόν παριστάνει σημείο με συντεταγμένες
\[ K\left(-\frac{A}{2},-\frac{B}{2}\right)\equiv K\left(-\frac{6}{2},-\frac{-10}{2}\right)\equiv K(-3,5) \]
\item Είναι $ A=-2,B=4 $ και $ \varGamma=9 $.
\[ A^2+B^2-4\varGamma=(-2)^2+4^2-4\cdot 9=4+16-36=-16<0 \]
Άρα η εξίσωση δεν παριστάνει κανένα σημείο.
\item Από την εξίσωση έχουμε ότι $ A=1,B=2 $ και $ \varGamma=\frac{1}{4} $ οπότε
\[ A^2+B^2-4\cdot\varGamma=1^2+2^2-4\cdot\frac{1}{4} \]
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-ExisApolTim-SolSE1-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Λύσεις ασκήσεων ενότητας/Εξισώσεις με απόλυτες τιμές/Alg-Exis1ou-ExisApolTim-SolSE1-1.tex','12/2/2022 16:54','Alg-Exis1ou-ExisApolTim-SectEx1','@article{Alg-Exis1ou-ExisApolTim-SectEx1_2,
	title = {Book title},
	author = {Bob Writer},
	publisher = {Book Publications},
	year = {2022},
	month = {1},
	isbn = {97852135484},
	pages = {524},
	series = {2},
}

@book{Alg-Exis1ou-ExisApolTim-SectEx1_2,
	title = {Book title},
	author = {Bob Writer},
	publisher = {Book Publications},
	year = {2022},
	month = {1},
	isbn = {97852135484},
	pages = {524},
	series = {2},
}

','%# Database File : Alg-Exis1ou-ExisApolTim-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item Πρέπει $ x+2\geq 0\Rightarrow x\geq -2 $. Έχουμε λοιπόν 
\[ |x-3|=x+2\Rightarrow x-3=\pm(x+2) \]
\begin{itemize}
\item $ x-3=x+2\Rightarrow x-x=2+3\Rightarrow 0x=5 $ αδύνατη.
\item $ x-3=-x-2\Rightarrow x+x=-2+3\Rightarrow 2x=1\Rightarrow x=\frac{1}{2} $ δεκτή λύση.
\end{itemize}
\item Πρέπει $ 2x-5\geq 0\Rightarrow 2x\geq 5\Rightarrow x\geq \frac{5}{2} $. Έχουμε
\[ |4x-1|=2x-5\Rightarrow 4x-1=\pm(2x-5) \]
\begin{itemize}
\item $ 4x-1=2x-5\Rightarrow 4x-2x=1-5\Rightarrow 2x=-4\Rightarrow x=-2 $ απορρίπτεται.
\item $ 4x-1=-2x+5\Rightarrow 4x+2x=5+1\Rightarrow 6x=6\Rightarrow x=1 $ απορρίπτεται.
\end{itemize}
άρα η εξίσωση είναι σδύνατη.
\item Πρέπει $ 4-7x\geq 0\Rightarrow -7x\geq-4\Rightarrow x\leq\frac{4}{7} $.
\[ |2x-3|=4-7x\Rightarrow 2x-3=\pm(4-7x) \]
\begin{itemize}
\item $ 2x-3=4-7x\Rightarrow 2x+7x=4+3\Rightarrow 9x=7\Rightarrow x=\frac{7}{9} $ δεκτή.
\item $ 2x-3=-4+7x\Rightarrow 2x-7x=-4+3\Rightarrow -5x=-1\Rightarrow x=\frac{1}{5} $ δεκτή.
\end{itemize}
\item Πρέπει $ \frac{x+3}{4}\geq 0\Rightarrow x+3\geq 0\Rightarrow x\geq -3 $.
\[ \left|\frac{x}{2}-1\right|=\frac{x+3}{4}\Rightarrow \frac{x}{2}-1=\pm\frac{x+3}{4} \]
\begin{itemize}
\item $\frac{x}{2}-1=\frac{x+3}{4}\Rightarrow 4\cdot\frac{x}{2}-4\cdot 1=4\cdot\frac{x+3}{4}\Rightarrow 2x-4=x+3\Rightarrow 2x-x=3+4\Rightarrow x=7$ δεκτή.
\item $\frac{x}{2}-1=-\frac{x+3}{4}\Rightarrow 4\cdot\frac{x}{2}-4\cdot 1=-4\cdot\frac{x+3}{4}\Rightarrow 2x-4=-x-3\Rightarrow 2x+x=-3+4\Rightarrow\\ 3x=1\Rightarrow x=\frac{1}{3}$ δεκτή.
\end{itemize}
\end{alist}
%# End of file Alg-Exis1ou-ExisApolTim-SolSE1-1','Basic','PdfLaTeX','Μια καινούρια άσκηση',0,'Α'' Λυκείου','','');
INSERT INTO "Database_Files" VALUES ('DTX-Analysh-Parag-Paragogos-Tab1','Tab','Analysh',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Ανάλυση/Παράγωγοι/Παράγωγος συνάρτηση/Πίνακες/DTX-Analysh-Parag-Paragogos-Tab1.tex','17/2/2022 10:56','-','@article{DTX-Analysh-Parag-Paragogos-Tab1,}','%# Database File : DTX-Analysh-Parag-Paragogos-Tab1
%@ Database source: Mathematics
\begin{longtable}{cc|ccc}
\hline \rule[-2ex]{0pt}{5.5ex} Συνάρτηση $ f $& Παράγωγος $ f'' $ & Συνάρτηση $ g\circ f $ & Παράγωγος $ \left( g\circ f \right)'' $ & Λεκτική περιγραφή \\ 
\hhline{=====} \rule[-2ex]{0pt}{5.5ex} $ c $ & $ 0 $ &  & & \\ 
\rule[-2ex]{0pt}{5ex} $ x $ & $ 1 $ &  & & \\ 
\rule[-2ex]{0pt}{5ex} $ x^\nu $ & $ \nu x^{\nu-1} $ & $ f^\nu(x) $ & $ \nu f^{\nu-1}(x)\cdot f''(x) $ & $ \nu(\text{βάση})^{\nu-1}(\text{βαση})'' $\\ 
\rule[-2ex]{0pt}{5ex} $ \dfrac{1}{x} $ & $ -\dfrac{1}{x^2} $ & $ \dfrac{1}{f(x)} $ & $ -\dfrac{f''(x)}{f^2(x)} $ &$ -\dfrac{(\text{Παρονομαστής})''}{\text{Παρονομαστής}^2} $ \\ 
\rule[-2ex]{0pt}{7ex} $ \sqrt{x} $ & $ \dfrac{1}{2\!\sqrt{x}} $ & $ \sqrt{f(x)} $ & $ \dfrac{f''(x)}{2\!\sqrt{f(x)}} $ & $ \dfrac{(\text{Υπόριζο})''}{2\cdot\text{Ρίζα}} $ \\ 
\rule[-2ex]{0pt}{5ex} $ \hm{x} $ & $ \syn{x} $ & $ \hm{f(x)} $ & $ \syn{f(x)}\cdot f''(x) $ & $ \syn{(\text{Γωνία})\cdot(\text{Γωνία})''} $\\ 
\rule[-2ex]{0pt}{5ex} $ \syn{x} $ & $ -\hm{x} $ & $ \syn{f(x)} $ & $ -\hm{f(x)}\cdot f''(x) $ &$ -\hm{(\text{Γωνία})\cdot(\text{Γωνία})''} $ \\ 
\rule[-2ex]{0pt}{5ex} $ \ef{x} $ & $ \dfrac{1}{\syn^2{x}} $ & $ \ef{f(x)} $ & $ \dfrac{f''(x)}{\syn^2{f(x)}} $ & $ \dfrac{(\text{Γωνία})''}{\syn^2{(\text{Γωνία})}} $ \\ 
\rule[-2ex]{0pt}{7ex} $ \syf{x} $ & $ -\dfrac{1}{\hm^2{x}} $ & $ \syf{f(x)} $ & $ -\dfrac{f''(x)}{\hm^2{f(x)}} $ & $ -\dfrac{(\text{Γωνία})''}{\hm^2{(\text{Γωνία})}} $ \\ 
\rule[-2ex]{0pt}{5ex} $ a^x $ & $ a^x\ln{a} $ & $ a^{f(x)} $ & $ a^{f(x)}\ln{a}\cdot f''(x) $ & $ a^{\text{Εκθέτης}}\cdot\ln{a}\cdot(\text{Εκθέτης})'' $ \\ 
\rule[-2ex]{0pt}{5ex} $ e^x $ & $ e^x $ & $ e^{f(x)} $ & $ e^{f(x)}\cdot f''(x) $ & $ e^{\text{Εκθέτης}}\cdot(\text{Εκθέτης})'' $ \\ 
\rule[-3ex]{0pt}{7ex} $ \ln{|x|} $ & $ \dfrac{1}{x} $ & $ \ln{|f(x)|} $ & $ \dfrac{f''(x)}{f(x)} $ & $ \dfrac{(\text{Παράσταση})''}{\text{Παράσταση}} $\\
\hline
\end{longtable}
%# End of file DTX-Analysh-Parag-Paragogos-Tab1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5.tex','17/2/2022 12:22','YES','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5
%@ Database source: Mathematics
Να βρεθεί η εξίσωση του κύκλου με κέντρο την αρχή των αξόνων $ O(0,0) $ 
\begin{alist}
\item και ακτίνα $ \rho=3 $.
\item ο οποίος διέρχεται από το σημείο $ A(-2,5) $.
\item ο οποίος εφάπτεται στην ευθεία $ \varepsilon : 6x-8y+3=0 $.
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5','Basic','PdfLaTeX','Εξίσωση κύκλου με γνωστά στοιχεία',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση εφαπτομένης κύκλου (αρχή αξόνων + τυχαία θέση)/DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1.tex','17/2/2022 12:26','NO','@article{DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1
%@ Database source: Mathematics
Σε καθένα από τα παρακάτω ερωτήματα να βρεθεί η εξίσωση της εφαπτομένης του κύκλου με κέντρο την αρχή των αξόνων $ O(0,0) $ στο σημείο $ M(x_0,y_0) $.
\begin{multicols}{2}
\begin{alist}
\item $ M(2,3) $
\item $ M(-1,4) $
\item $ M(5,-2) $
\item $ M(0,8) $
\end{alist}
\end{multicols}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1','Basic','PdfLaTeX','Εξίσωση εφαπτομένης κύκλου με κέντρο την αρχή των αξόνων. Δεν δίνεται η ακτίνα του κύκλου.',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση εφαπτομένης κύκλου (αρχή αξόνων + τυχαία θέση)/DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2.tex','17/2/2022 12:31','NO','@article{DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2
%@ Database source: Mathematics
Να βρεθεί η εξίσωση του κύκλου με κέντρο την αρχή των αξόνων $ O(0,0) $ στο δοσμένο σημείο επαφής $ M $.
\begin{alist}
\item $ x^2+y^2=13,\ M(2,3) $
\item $ x^2+y^2=4,\ M(1,\sqrt{3}) $
\item $ x^2+y^2=8,\ M(-2,2) $
\item $ x^2+y^2=9,\ M(3,0) $
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2','Basic','PdfLaTeX','Εξίσωση εφαπτομένης με γνωστό σημείο επαφής και γνωστή εξίσωση κύκλου.',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση εφαπτομένης κύκλου (αρχή αξόνων + τυχαία θέση)/DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3.tex','17/2/2022 12:37','NO','@article{DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3
%@ Database source: Mathematics
Να βρεθεί η εξίσωση της εφαπτομένης του κύκλου $ x^2+y^2=20 $
\begin{alist}
\item στο σημείο $ A(4,2) $
\item η οποία είναι παράλληλη με την ευθεία $ \zeta: y=2x-3 $.
\item η οποία διέρχεται από το σημείο $ P(1,8) $
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση εφαπτομένης κύκλου (αρχή αξόνων + τυχαία θέση)/DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4.tex','17/2/2022 12:47','NO','@article{DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4
%@ Database source: Mathematics
Να βρεθεί η εξίσωση της εφαπτομένης του δοσμένου κύκλου στο σημείο $ M $.
\begin{alist}
\item $ (x-3)^2+(y+2)^2=5,\ M(4,0) $
\item $ (x+1)^2+(y-2)^2=10,\ M(2,3) $
\item $ (x-2)^2+(y-5)^2=9,\ M(-1,5) $
\item $ (x-3)^2+y^2=20,\ M(1,-2) $
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6.tex','17/2/2022 12:53','NO','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6
%@ Database source: Mathematics
Να βρεθεί η εξίσωση του κύκλου που διέρχεται από τα σημεία $ A(-1,1),B(2,4) $ και $ \varGamma(5,-2) $.
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6','Basic','PdfLaTeX','Εξίσωση κύκλου που διέρχεται από 3 σημεία.',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Γεωμετρικές εφαρμογές/DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1.tex','17/2/2022 13:01','NO','@article{DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1
%@ Database source: Mathematics
Να βρεθεί η εξίσωση του περιγεγραμμένου κύκλου του τριγώνου $ AB\varGamma $ με κορυφές $ A(2,3),B(-4,0) $ και $ \varGamma(-1,-3) $.
%# End of file DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE5-1','SolSE','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Λύσεις ασκήσεων ενότητας/Εξίσωση κύκλου (αρχή αξόνων - τυχαίο κέντρο)/DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE5-1.tex','17/2/2022 13:02','-','@article{DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE5-1
%@ Database source: Mathematics
\begin{alist}
\item $ x^2+y^2=\rho^2\Rightarrow x^2+y^2=3^2\Rightarrow x^2+y^2=9 $.
\item Η ακτίνα του κύκλου ισούται με $ \rho=KA=\sqrt{x_A^2+y_A^2}=\sqrt{(-2)^2+5^2}=\sqrt{4+25}=\sqrt{29} $. Άρα η εξίσωση του κύκλου είναι
\[ x^2+y^2=\rho^2\Rightarrow x^2+y^2=\sqrt{29}^2\Rightarrow x^2+y^2=29 \]
\item Η ακτίνα του κύκλου ισούται με την απόσταση του κέντρου από την ευθεία. Άρα
\[ \rho=d(O,\varepsilon)=\frac{|6\cdot 0-8\cdot 0+3|}{\sqrt{6^2+(-8^2)}}=\frac{3}{\sqrt{100}}=\frac{3}{10} \]
οπότε η εξίσωση του κύκλου θα είναι
\[ x^2+y^2=\rho^2\Rightarrow x^2+y^2=\left(\frac{3}{10}\right)^2\Rightarrow x^2+y^2=\frac{9}{100} \]
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE5-1','Basic','PdfLaTeX','Εξίσωση κύκλου με γνωστά στοιχεία',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5','SectEx','Geometria',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Γεωμετρία/Κωνικές τομές/Κύκλος - Εξίσωση κύκλου/Ασκήσεις/Εξίσωση εφαπτομένης κύκλου (αρχή αξόνων + τυχαία θέση)/DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5.tex','17/2/2022 13:57','NO','@article{DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5,}','%# Database File : DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5
%@ Database source: Mathematics
Να βρεθεί η εξίσωση της εφαπτομένης του κύκλου $ (x-3)^2+(y+1)^2=5 $
\begin{alist}
\item στο σημείο επαφής $ M(4,1) $.
\item η οποία είναι παράλληλη με την ευθεία $ \zeta: y=-\frac{x}{2}+3 $.
\item η οποία διέρχεται από το σημείο $ P(-2,0) $.
\end{alist}
%# End of file DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE1-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Λύσεις ασκήσεων ενότητας/Παραμετρικές/Alg-GrammikaSys-Parametrikes-SolSE1-1.tex','21/2/2022 17:40','-','','%# Database File : Alg-GrammikaSys-Parametrikes-SolSE1-1
%@ Database source: Mathematics
Η εξίσωση παριστάνει ευθεία για κάθε $ \lambda\in\mathbb{R} $ αφού
\[ \lambda=0\ \ \text{και}\ \ \lambda-1=0\Rightarrow\lambda=1 \]
δηλαδή οι συντελεστές των $ x,y $ δεν μηδενίζονται συγχρόνως. Στη συνέχεια έχουμε ότι το σημείο $ A(-2,3) $ ανήκει στην ευθεία αυτή αν και μόνο αν για $ x=-2 $ και $ y=3 $
\[ \lambda\cdot(-2)+(\lambda-1)\cdot 3=4\Rightarrow -2\lambda+3\lambda-3=4\Rightarrow \lambda=7 \]
%# End of file Alg-GrammikaSys-Parametrikes-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE3-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Λύσεις ασκήσεων ενότητας/Παραμετρικές/Alg-GrammikaSys-Parametrikes-SolSE3-1.tex','21/2/2022 17:48','-','','%# Database File : Alg-GrammikaSys-Parametrikes-SolSE3-1
%@ Database source: Mathematics
Αφού το σημείο $ A $ ανήκει στην ευθεία τότε για $ x=3\lanbda-1 $ και $ y=4-\lambda $ έχουμε
\begin{gather*}
2\cdot(3\lambda-1)+3\cdot(4-\lambda)=1\Rightarrow\\
6\lambda-2+12-3\lambda=1\Rightarrow\\
6\lambda-3\lambda=2-12+1\Rightarrow\\
3\lambda=-9\Rightarrow \lambda =-3
\end{gather*}
%# End of file Alg-GrammikaSys-Parametrikes-SolSE3-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-GrammikaSys-MethAnt-SolSE5-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Γραμμικά συστήματα/Λύσεις ασκήσεων ενότητας/Μέθοδος αντικατάστασης/Alg-GrammikaSys-MethAnt-SolSE5-1.tex','21/2/2022 18:06','-','','%# Database File : Alg-GrammikaSys-MethAnt-SolSE5-1
%@ Database source: Mathematics
\begin{alist}
\item 
\end{alist}
%# End of file Alg-GrammikaSys-MethAnt-SolSE5-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Πολυώνυμα/Ασκήσεις/Τιμή - Ρίζα πολυωνύμου/DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2.tex','21/2/2022 18:19','NO','@article{DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2,}','%# Database File : DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2
%@ Database source: Mathematics
Δίνεται το πολυώνυμο $ P(x)=ax^3+(2a-\beta)x^2+4x-3\beta+5 $ για το οποίο ισχύει $ P(-2)=-24 $ ενώ το $ 1 $ είναι ρίζα του πολυωνύμου.
\begin{alist}
\item Να βρεθούν οι τιμές των παραμέτρων $ a,\beta $.
\item 
\end{alist}
%# End of file DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Ταυτότητες/Ασκήσεις/Βασικές πολυωνυμικές ταυτότητες/DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1.tex','21/2/2022 20:40','NO','@article{DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1,}','%# Database File : DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1
%@ Database source: Mathematics
Να βρεθούν τα αναπτύγματα από τις παρακάτω ταυτότητες.
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.]
\item $ \left(x+2 \right)^2 $
\item $ \left(2x+1 \right)^2 $
\item $ \left(3x+2y \right)^2 $
\item $ \left(x^2+x \right)^2 $
\item $ \left(4x+5y \right)^2 $
\item $ \left(x^2+3x \right)^2 $
\item $ \left(\dfrac{x}{2}+2 \right)^2 $
\item $ \left(\dfrac{1}{x}+\dfrac{x}{2} \right)^2 $
\item $ \left(\sqrt{x}+\sqrt{y} \right)^2 $
\end{enumerate}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Ταυτότητες/Ασκήσεις/Βασικές πολυωνυμικές ταυτότητες/DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2.tex','21/2/2022 20:41','NO','@article{DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2,}','%# Database File : DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2
%@ Database source: Mathematics
Να βρεθούν τα αναπτύγματα από τις παρακάτω ταυτότητες.
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.]
\item $ \left(x-3 \right)^2 $
\item $ \left(2x-5 \right)^2 $
\item $ \left(3x-y \right)^2 $
\item $ \left(x-x^2 \right)^2 $
\item $ \left(4x-5y \right)^2 $
\item $ \left(x^2-7x \right)^2 $
\item $ \left(\dfrac{x}{4}-1 \right)^2 $
\item $ \left(\dfrac{2}{x}-\dfrac{x}{3} \right)^2 $
\item $ \left(\sqrt{x}-2 \right)^2 $
\end{enumerate}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Ταυτότητες/Ασκήσεις/Βασικές πολυωνυμικές ταυτότητες/DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3.tex','21/2/2022 20:42','NO','@article{DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3,}','%# Database File : DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3
%@ Database source: Mathematics
Να βρεθούν τα αναπτύγματα από τις παρακάτω ταυτότητες.
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.]
\item $ \left(x+3 \right)^3 $
\item $ \left(2x+3 \right)^3 $
\item $ \left(4x+3y \right)^3 $
\item $ \left(x^2+1 \right)^3 $
\item $ \left(\dfrac{x}{2}+\dfrac{2}{x} \right)^3 $
\item $ \left(y+\dfrac{1}{y}\right)^3 $
\end{enumerate}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Ταυτότητες/Ασκήσεις/Βασικές πολυωνυμικές ταυτότητες/DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4.tex','21/2/2022 20:42','NO','@article{DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4,}','%# Database File : DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4
%@ Database source: Mathematics
Να βρεθούν τα αναπτύγματα από τις παρακάτω ταυτότητες.
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.]
\item $ \left(x-4 \right)^3 $
\item $ \left(3x-1 \right)^3 $
\item $ \left(4x-5y \right)^3 $
\item $ \left(z^2-1 \right)^3 $
\item $ \left(\dfrac{y}{3}-\dfrac{3}{y} \right)^3 $
\item $ \left(a-\dfrac{1}{a}\right)^3 $
\end{enumerate}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Παραγοντοποίηση/Ασκήσεις/Διαφορά τετραγώνων/DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1.tex','21/2/2022 20:43','NO','@article{DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1,}','%# Database File : DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1
%@ Database source: Mathematics
Να παραγοντοποιηθούν οι παρακάτω παραστάσεις.
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.]
\item $ x^2-9 $
\item $ 4x^2-25 $
\item $ 9x^2-16 $
\item $ 36y^2-49 $
\item $ 64y^2-81z^2 $
\item $ x^4-1 $
\end{enumerate}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Παραγοντοποίηση/Ασκήσεις/Άθροισμα - Διαφορά κύβων/DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1.tex','21/2/2022 20:44','NO','@article{DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1,}','%# Database File : DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1
%@ Database source: Mathematics
Να παραγοντοποιηθούν οι παρακάτω παραστάσεις
\begin{multicols}{2}
\begin{enumerate}[label=\roman*.]
\item $ x^3+27 $
\item $ y^3+125 $
\item $ z^3+64 $
\item $ 8x^3+125 $
\end{enumerate}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Αλγεβρικές παραστάσεις/Παραγοντοποίηση/Ασκήσεις/Άθροισμα - Διαφορά κύβων/DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2.tex','21/2/2022 20:45','NO','@article{DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2,}','%# Database File : DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2
%@ Database source: Mathematics
Να παραγοντοποιηθούν οι παρακάτω παραστάσεις
\begin{multicols}{3}
\begin{enumerate}[label=\roman*.]
\item $ x^3-8 $
\item $ 125-z^3 $
\item $ 27x^3-64 $
\item $ 8x^3-125z^3 $
\end{enumerate}
\end{multicols}
%# End of file DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Εξισώσεις - Ανισώσεις/Εξισώσεις 2ου βαθμού/Ασκήσεις/Παραμετρικές/DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1.tex','24/2/2022 11:26','NO','@article{DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1,}','%# Database File : DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1
%@ Database source: Mathematics
Δίνεται η εξίσωση 
\[ x^2-\lambda x+\lambda-1=0 \]
όπου $ \lambda\in\mathbb{R} $. Να λυθεί η εξίσωση για τις διάφορες τιμές του $ \lambda $.
%# End of file DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2','SectEx','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Εξισώσεις - Ανισώσεις/Εξισώσεις 2ου βαθμού/Ασκήσεις/Παραμετρικές/DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2.tex','24/2/2022 11:43','YES','@article{DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2,}','%# Database File : DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2
%@ Database source: Mathematics
Δίνεται η εξίσωση
\[ (\lambda-1)x^2+(\lambda-2)x-1=0 \]
όπου $ \lambda\in\mathbb{R} $. Να λυθεί η εξίσωση για τις διάφορες τιμές του $ \lamnda $.
%# End of file DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2','Basic','PdfLaTeX','',0,NULL,NULL,NULL);
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SolSE1-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Λύσεις ασκήσεων ενότητας/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SolSE1-1.tex','25/2/2022 19:45','-','','%# Database File : Alg-Logarithmos-YpolLog-SolSE1-1
%@ Database source: Mathematics
\begin{multicols}{2}
\begin{rlist}
\item $ \log_2{4}=\log_2{2^2}=2 $
\item $ \log_3{9}=\log_3{3^2}=2 $
\item $ \log_5{125}=\log_5{5^3}=3 $
\item $ \log_2{16}=\log_2{2^4}=4 $
\item $ \log_3{27}=\log_3{3^3}=3 $
\item $ \log_4{16}=\log_4{4^2}=2 $
\item $ \log_2{32}=\log_2{2^5}=5 $
\item $ \log_2{64}=\log_2{2^6}=6 $
\end{rlist}
\end{multicols}
%# End of file Alg-Logarithmos-YpolLog-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SolSE10-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Λύσεις ασκήσεων ενότητας/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SolSE10-1.tex','25/2/2022 19:51','-','','%# Database File : Alg-Logarithmos-YpolLog-SolSE10-1
%@ Database source: Mathematics
\begin{alist}
\item Θέτω $ \log_2(\sqrt[3]{2})=x $ άρα
\begin{align*}
\log_2{(\sqrt[3]{2})}=x&\Rightarrow 2^x=\sqrt[3]{2}\Rightarrow\\
&\Rightarrow 2^x=2^{\frac{1}{3}}\Rightarrow x=\frac{1}{3}
\end{align*}
άρα $ \log_2(\sqrt[3]{2})=\frac{1}{3} $.
\item Θέτω $ \log_{2}{\sqrt[4]{8}}=x $
\[ \log_2{\sqrt[4]{8}}=x\Rightarrow 2^x=\sqrt[4]{8}\Rightarrow 2^x=\sqrt[4]{2^3}\Rightarrow 2^x=2^{\frac{3}{4}}\Rightarrow x=\frac{3}{4} \]
άρα $ \log_{2}{\sqrt[4]{8}}=\frac{3}{4} $.
\item Θέτω $ \log_{3}{\sqrt[3]{9}}=x $
\[ \log_{3}{\sqrt[3]{9}}=x\Rightarrow 3^x=\sqrt[3]{9}\Rightarrow 3^x=\sqrt[3]{3^2}\Rightarrow 3^x=3^{\frac{2}{3}}\Rightarrow x=\frac{2}{3} \]
άρα $ \log_{3}{\sqrt[3]{9}}=\frac{2}{3} $.
\item Θέτω $\log_{4}{\frac{1}{2\sqrt[5]{16}}}=x$
\begin{align*}
\log_{4}{\frac{1}{2\sqrt[5]{16}}}=x&\Rightarrow 4^x=2\sqrt[5]{16}\Rightarrow\\
&\Rightarrow 4^x=2\sqrt[5]{2^4}\Rightarrow\\
&\Rightarrow (2^2)^x=2\cdot 2^{\frac{5}{4}}\Rightarrow\\
&\Rightarrow 2^{2x}=2^{\frac{9}{4}}\Rightarrow 2x=\frac{9}{4}\Rightarrow x=\frac{9}{8}
\end{align*}
άρα $ \log_{4}{\frac{1}{2\sqrt[5]{16}}}=\frac{9}{8} $.
\item Θέτω $ \log_{5}{\sqrt[3]{5}}=x $
\[ \log_{5}{\sqrt[3]{5}}=x\Rightarrow 5^x=\sqrt[3]{5}\Rightarrow 5^x=5^{\frac{1}{3}}\Rightarrow x=\frac{1}{3} \]
άρα $ \log_{5}{\sqrt[3]{5}}=\frac{1}{3} $.
\item Θέτω $ \log_{5}{\frac{1}{\sqrt{125}}}=x $
\[ \log_{5}{\frac{1}{\sqrt{125}}}=x\Rightarrow 5^x=\frac{1}{\sqrt{125}}\Rightarrow 5^x=\frac{1}{\sqrt{5^3}}\Rightarrow 5^x=5^{-\frac{3}{2}}\Rightarrow x=-\frac{3}{2} \]
άρα $ \log_{5}{\frac{1}{\sqrt{125}}}=-\frac{3}{2} $.
\end{alist}
%# End of file Alg-Logarithmos-YpolLog-SolSE10-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-AorAdynEx-SolSE1-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Λύσεις ασκήσεων ενότητας/Αόριστες και αδύνατες εξισώσεις 1ου βαθμού/Alg-Exis1ou-AorAdynEx-SolSE1-1.tex','03/3/2022 00:24','-','','%# Database File : Alg-Exis1ou-AorAdynEx-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item $ 3x+4=2(x-1)+x-5\Rightarrow 3x+4=2x-2+x-5\Rightarrow 3x-2x-x=-2-5-4\Rightarrow 0x=-11 $ αδύνατη.
\item $ 4x-(3-2x)=2x+9\Rightarrow 4-3+2x=2x+9\Rightarrow 2x-2x=9+3-4\Rightarrow 0x=8 $ αδύνατη.
\item $ 8-(4-x)=2x-(x-3)\Rightarrow 8-4+x=2x-x+3\Rightarrow x+x-2x=-8+4+3\Rightarrow 0x=-1 $ αδύνατη. 
\item $ 2(x+3)-14=21-5(3-x)-3x\Rightarrow 2x+6-14=21-15+5x-3x\Rightarrow 2x-5x+3x=14-6+21-15\Rightarrow 0x=14 $ αδύνατη.
\end{alist}
%# End of file Alg-Exis1ou-AorAdynEx-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SolSE4-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Λύσεις ασκήσεων ενότητας/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SolSE4-1.tex','03/3/2022 22:29','-','','%# Database File : Alg-Logarithmos-YpolLog-SolSE4-1
%@ Database source: Mathematics
\begin{alist}
\item $ \log{\frac{1}{100}}=\log{\frac{1}{10^2}}=\log{10^{-2}}=-2 $.
\item $ \log{\frac{1}{1000}}=\log{\frac{1}{10^3}}=\log{10^{-3}}=-3 $.
\item $ \log{\frac{1}{10^{-3}}}=\log{10^3}=3 $.
\item $ \ln{\frac{1}{e}}=\ln{e^{-1}}=-1 $.
\item $ \ln{\frac{1}{e^5}}=\ln{e^{-5}}=-5 $.
\item $ \ln{\frac{1}{e^{-4}}}=\ln{e^4}=4 $.
\end{alist}
%# End of file Alg-Logarithmos-YpolLog-SolSE4-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-Logarithmos-YpolLog-SolSE5-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Λογάριθμος/Λύσεις ασκήσεων ενότητας/Υπολογισμός λογαρίθμων/Alg-Logarithmos-YpolLog-SolSE5-1.tex','03/3/2022 22:39','-','','%# Database File : Alg-Logarithmos-YpolLog-SolSE5-1
%@ Database source: Mathematics
\begin{alist}
\item $ \log_2{0{,}25}=\log_2{\frac{25}{100}}=\log_2{\frac{1}{4}}=\log_2{\frac{1}{2^{2}}}=\log_2{2_{-2}}=-2 $.
\item $ \log_2{0{,}125}=\log_2{\frac{125}{1000}}=\log_2{\frac{1}{8}}=\log{\frac{1}{2^3}}=\log_2{2^{-3}}=-3 $.
\item $ \log_5{0{,}04}=\log_5{\frac{4}{100}}=\log_5{\frac{1}{25}}=\log_5{\frac{1}{5^2}}=\log_5{5^{-2}}=-2 $.
\item $ \log_8{0{,}125}=\log_8{\frac{125}{1000}}=\log_8{\frac{1}{8}}=\log_8{8^{-1}}=-1 $.
\item $ \log{0{,}0001}=\log{\frac{1}{10000}}=\log{\frac{1}{10^4}}=\log{10^{-4}}=-4 $.
\item $ \log_{100}{0{,}01}=\log_{100}{\frac{1}{100}}=\log_{100}{100^{-1}}=-1 $.
\end{alist}
%# End of file Alg-Logarithmos-YpolLog-SolSE5-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('Alg-Exis1ou-EpilExis-SolSE1-1','SolSE','Algebra',0,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Ενότητες/Εξισώσεις 1ου βαθμού/Λύσεις ασκήσεων ενότητας/Επίλυση απλής πολυωνυμικής εξίσωσης/Alg-Exis1ou-EpilExis-SolSE1-1.tex','03/3/2022 22:53','-','','%# Database File : Alg-Exis1ou-EpilExis-SolSE1-1
%@ Database source: Mathematics
\begin{alist}
\item $ 2x-4=0\Rightarrow 2x=4\Rightarrow \frac{2x}{2}=\frac{4}{2}\Rightarrow x=2 $.
\item $ 12-4x=0\Rightarrow -4x=-12\Rightarrow \frac{-4x}{-4}=\frac{-12}{-4}\Rightarrow x=3 $.
\item $ 3x+4=x-8\Rightarrow 3x-x=-4-8\Rightarrow 2x=-12\Rightarrow \frac{2x}{2}=\frac{-12}{2}\Rightarrow x=-6 $.
\item $ 5x-7=2x+5\Rightarrow 5x-2x=5+7\Rightarrow 3x=12\Rightarrow \frac{3x}{3}=\frac{12}{3}\Rightarrow x=4 $.
\end{alist}
%# End of file Alg-Exis1ou-EpilExis-SolSE1-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "Database_Files" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SolSE2-1','SolSE','Algebra',1,'/home/spyros/Μαθηματικά/Βάση Δεδομένων Μαθηματικών/Άλγεβρα/Εξισώσεις - Ανισώσεις/Εξισώσεις 2ου βαθμού/Λύσεις ασκήσεων ενότητας/Παραμετρικές/DTX-Algebra-ExAn-Exis2ou-Parametrikes-SolSE2-1.tex','08/3/2022 12:15','-','@article{DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2,}','%# Database File : DTX-Algebra-ExAn-Exis2ou-Parametrikes-SolSE2-1
%@ Database source: Mathematics
Η εξίσωση είναι της μορφής $ ax^2+\beta x+\gamma=0 $ με $a=\lambda-1,\beta=\lambda-2$ και $ \gamma=-1 $. Δια κρίνουμε τις εξής περιπτώσεις:
\begin{alist}
\item Αν $\lambda-1\neq 0\Rightarrow \lambda\neq 1$ τότε η εξίσωση είναι 2ου βαθμού. Θα έχουμε λοιπόν
\[\Delta=\beta^2-4a\gamma=(\lambda-2)^2-4\cdot(\lambda-1)\cdot(-1)=\lambda^2-4\lambda+3+4\lambda-4=\lambda^2\geq 0\]
\begin{itemize}
\item Αν $ \Delta>0\Rightarrow \lambda^2>0\Rightarrow \lambda\neq 0 $ τότε η εξίσωση έχει 2 άνισες λύσεις
\[ x_{1,2}=\frac{-\beta\pm\sqrt{\Delta}}{2a}=\frac{-(\lambda-2)\pm\sqrt{\lambda^2}}{2(\lambda-1)}=\frac{-\lambda+2\pm\lambda}{2(\lambda-1)} \]
άρα θα είναι
\[ x_1=\frac{-\lambda+2+\lambda}{2(\lambda-1)}=\frac{2}{2(\lambda-1)}=\frac{1}{\lambda-1}\ \ ,\ \ x_2=\frac{-\lambda+2-\lambda}{2(\lambda-1)}=\frac{-2\lambda+2}{2(\lambda-1)}=\frac{-2(\lambda-1)}{2(\lambda-1)}=-1 \]
\item Αν $ \Delta=0\Rightarrow \lambda^2=0\Rightarrow \lambda=0 $ τότε η εξίσωση έχει μια διπλή λύση
\[ x=-\frac{\beta}{2a}=-\frac{\lambda-2}{2(\lambda-1)}=-\frac{0-2}{2(0-1)}=-1 \]
\end{itemize}
\item 
\end{alist}
%# End of file DTX-Algebra-ExAn-Exis2ou-Parametrikes-SolSE2-1','Basic','PdfLaTeX','',0,'','','');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-ExisApolTim-SectEx1','ExisApolTim');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Exiswsh_2Ou_Bathmou','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Geometrikh_Proodos','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Tautothta','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Tetragwnikh_Riza','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Dynamh_Pragm_Aritmou','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Yposynolo','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Akolouthia','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Dynamh_Me_Rhto_Ektheth','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Arithmitikh_Proodos','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-N_Osth_Riza','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Atithmitikos_Mesos','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Apolyth_Timh','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Synolo','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Or-Isa_Synola','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-AorAdynEx-SectEx1','AorAdynEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx1','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx4','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx3','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-EpilExis-SectEx2','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx1','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx2','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx4','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx5','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx3','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx9','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx8','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx10','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx7','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SectEx6','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx1','GenOrArPr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx3','GenOrArPr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx2','GenOrArPr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ArProodos-GenOrArPr-SectEx4','GenOrArPr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-AnisApT-SectEx1','AnisApT');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-AnisApT-SectEx2','AnisApT');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx3','KoinLys');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx1','KoinLys');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-KoinLys-SectEx2','KoinLys');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-SwstoLathos-SectEx1','SwstoLathos');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-ErwTheor-SectEx1','ErwTheor');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx4','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx3','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx1','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-EpilAnis-SectEx2','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE2','AnisApT');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE1','AnisApT');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx1','TrExArnAr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-TrExArnAr-SectEx2','TrExArnAr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx2','AplTrExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx1','AplTrExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-AplTrExis-SectEx3','AplTrExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx6','SynthTrEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx1','SynthTrEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx2','SynthTrEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx5','SynthTrEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx3','SynthTrEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigExisoseis-SynthTrEx-SectEx4','SynthTrEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx1','MhkKenAkt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx4','MhkKenAkt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx2','MhkKenAkt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx5','MhkKenAkt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ApolTimh-MhkKenAkt-SectEx3','MhkKenAkt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigArithmoi-TrigBasGwn-SectEx1','TrigBasGwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigArithmoi-TrigSysSyn-SectEx1','TrigSysSyn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigArithmoi-TrAr2pi+-SectEx1','TrAr2pi+');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx2','MoirAkt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigArithmoi-MoirAkt-SectEx1','MoirAkt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx2','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-TrigArithmoi-Problhmata-SectEx1','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx2','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx1','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SectEx3','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx5','MethAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx4','MethAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx3','MethAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx1','MethAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SectEx2','MethAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethOriz-SectEx2','MethOriz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethOriz-SectEx1','MethOriz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx4','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx7','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx2','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx3','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx8','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx6','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx1','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Problhmata-SectEx5','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx1','GramExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx3','GramExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx4','GramExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-GramExis-SectEx2','GramExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx1','GrafEpil');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-GrafEpil-SectEx2','GrafEpil');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAntS-SectEx2','MethAntS');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAntS-SectEx1','MethAntS');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx2','ExisAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx3','ExisAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx4','ExisAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-ExisAnt-SectEx1','ExisAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx1','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx3','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-Parametrikes-SectEx2','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-Problhmata-SectEx1','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx2','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx3','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx1','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EpilExis-SectEx4','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-SwstoLathos-SectEx1','SwstoLathos');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx5','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx1','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx4','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx6','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx2','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-EidhRizwn-SectEx3','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx5','TypoiVieta');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx6','TypoiVieta');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx1','TypoiVieta');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx2','TypoiVieta');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx4','TypoiVieta');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis2ou-TypoiVieta-SectEx3','TypoiVieta');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-KoinLys-SectEx1','KoinLys');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx1','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx5','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx3','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx4','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SectEx2','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx2','ParagTriwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx4','ParagTriwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx1','ParagTriwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-ParagTriwn-SectEx3','ParagTriwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx2','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EidhRizwn-SectEx1','EidhRizwn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE1','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE2','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis2ou-EpilAnis-SolSE3','EpilAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx7','IdiotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx5','IdiotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx4','IdiotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx1','IdiotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx3','IdiotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx6','IdiotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-IdiotAnis-SectEx2','IdiotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx4','SygkrAr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx3','SygkrAr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx2','SygkrAr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-SygkrAr-SectEx1','SygkrAr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-ApodAnis-SectEx1','ApodAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-ApodAnis-SectEx2','ApodAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-MidAthr-SectEx1','MidAthr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx4','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx2','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx6','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx3','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx9','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx5','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx1','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx7','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Diataxh-Diasthmata-SectEx8','Diasthmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_an_diasthma','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synarthsh','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh_se_kl_diasthma','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Grafikh_Parastash','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Topika_Akrotata','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Elaxisto','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Deuterh-N-osth_Paragwgos','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_shmeio','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Rythmos_Metabolhs','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Efaptomenh_Graf_Parastashs','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Prakseis_Synarthsewn','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_an_diasthma','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Paragwgisimh_Synarthsh','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synolo_Timwn','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Gnisiws_Fthinousa','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synthesh_Synarthsewn','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_kl_diasthma','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synexhs_Synarthsh_se_synolo','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Synarthsh_1-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Ises_Synarthseis_1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Gnisiws_Auxousa','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Pragmatikh_Synarthsh','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Oliko_Megisto','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Olika_Akrotata','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Gnisiws_Monotonh','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Paragwgos_synarthsh','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Paragwgos_se_shmeio','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Topiko_Elaxisto','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Or-Topiko_Megisto','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-MonotAkrot-MonAkrGrPar-SectEx1','MonAkrGrPar');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-MonotAkrot-Monoton-SectEx1','Monoton');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-MonotAkrot-AkrotAnis-SectEx1','AkrotAnis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx5','PedioOrism');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx1','PedioOrism');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx2','PedioOrism');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx3','PedioOrism');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-PedioOrism-SectEx4','PedioOrism');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx8','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx14','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx2','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx5','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx11','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx16','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx15','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx3','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx6','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx12','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx9','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx1','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx13','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx7','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx4','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-LogarSyn-LogarExis-SectEx10','LogarExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-MonadRiz-SectEx1','MonadRiz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizKlDiast-SectEx1','RizKlDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-BolzLim-SectEx1','BolzLim');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx2','RizaExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx1','RizaExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizaExis-SectEx3','RizaExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx1','RizAnDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx3','RizAnDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SectEx2','RizAnDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-NRizes-SectEx1','NRizes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-MonadRiz-SolSE1','MonadRiz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizKlDiast-SolSE1','RizKlDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-BolzLim-SolSE1','BolzLim');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE3','RizaExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE1','RizaExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizaExis-SolSE2','RizaExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE3','RizAnDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE2','RizAnDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-RizAnDiast-SolSE1','RizAnDiast');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-ThBolzano-NRizes-SolSE1','NRizes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-OrioShmeio-Or0p0Riz-SectEx1','Or0p0Riz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx3','Orio0p0Pol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx1','Orio0p0Pol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx4','Orio0p0Pol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-OrioShmeio-Orio0p0Pol-SectEx2','Orio0p0Pol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-OrioShmeio-Or0p0Riz-SolSE1','Or0p0Riz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParPollTyp-SectEx2','ParPollTyp');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParPollTyp-SectEx1','ParPollTyp');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx1','ParagPilik');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx3','ParagPilik');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagPilik-SectEx2','ParagPilik');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParSynthRiz-SectEx1','ParSynthRiz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx7','ParagSynth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx1','ParagSynth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx3','ParagSynth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx6','ParagSynth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx2','ParagSynth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx4','ParagSynth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagSynth-SectEx5','ParagSynth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagGin-SectEx1','ParagGin');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagGin-SectEx2','ParagGin');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParSynEkth-SectEx1','ParSynEkth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParSynEkth-SectEx2','ParSynEkth');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx1','ParAnTax');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx2','ParAnTax');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParAnTax-SectEx3','ParAnTax');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Paragogos-ParagGin-SolSE1','ParagGin');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Th-Grafikes_Par_Cf_Cf-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-Method-Select file name','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-ExAn-Exis1ou-EpilExis-SectEx2','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Anis1ouAnis2ou-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Anis1ouAnis2ou-SolCE1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Pithanothtes-Table-Endexomena','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Table-IdiothtesDynamewn','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Table-IdiothtesApolytwnTimwn','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Table-IdiothtesRizwn','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Figure-Diagramma_DionymEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Table-EidhRizwn','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx1','MonotPar');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Ana-Parag-MonotAkrot-MonotPar-SectEx2','MonotPar');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-CombEx3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Theorem-Proshmo_Triwnymou','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-Table-Eidh_Synarthsewn_1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Algebra-Table-Typoi_Ar_Geom_Proodou','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE4','AnisApT');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis3ou+-Method-(Select a name here)','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-AntistrSynSynarthshMonotAkrotSynart1p1SynexeiaSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-GrParastashThETΜΕΤSynthSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-IsesSynMonotAkrotOrioShmeioSynthSyn-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-ThBolzanoMonotAkrotParagogos-Theoria-CombSub1','Theoria');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Anis1ou-AnisApT-SolSE1-3','AnisApT');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE2-1-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE3-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE3-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE1-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Analysh-KyrtothtaMonotAkrot-SolCE1-1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Definition2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Definition3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Theorem3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Method1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Example1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-EmbadaBasSxPythTheor-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-Problhmata-SectEx1','Problhmata');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-Pithanothta-Probl-SectEx1','Probl');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition4','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition5','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition6','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-Definition7','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Theorem1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-ParStD-Definition4','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-Definition4','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraThesis-Definition3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SectEx1','PrEnd');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Arith-Diataxh-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Akol-ArProodos-Definition1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Pithanothtes-Pithan-DeigmEnd-PrEnd-SolSE1-1','PrEnd');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE2-1','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Definition2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AkolouthAnis1ou-CombEx1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-AkAnadTyp-SectEx1','AkAnadTyp');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Akol-Akolouth-Prob1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-ParagGin-SectEx1','ParagGin');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-ParagPilik-SectEx1','ParagPilik');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Synart-AntistrSyn-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Oria-Asymptotes-Def3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Olokl-OrismOlokl-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Kyrtothta-Def4','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Synart-Synarthsh-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-MonotAkrot-Def4','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-ParagShmeio-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-RythMetab-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def4','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def5','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def6','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def7','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def8','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-PaStD-BasEnnStat-Def9','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx1','GramExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-GramExis-SectEx2','GramExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx1','OrSys');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-OrSys-SectEx2','OrSys');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx1','MethAntS');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethAntS-SectEx2','MethAntS');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Syst-GrammikaSys-MethOriz-SectEx1','MethOriz');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx1','OrismPol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-OrismPol-SectEx2','OrismPol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx1','BathmPol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx1','TimhPol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-BathmPol-SectEx2','BathmPol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-PraksPol-SectEx1','PraksPol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx1','SyntGnAk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SectEx2','SyntGnAk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SectEx1','SyntDian');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx1','SyntMes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx1','IsaMhdPar');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-IsaMhdPar-SectEx2','IsaMhdPar');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx1','MetroDian');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx2','MetroDian');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SectEx2','SyntMes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx1','SyntelDieu');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGrSynd-SectEx1','SyntGrSynd');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-MetrGrSyn-SectEx1','MetrGrSyn');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntelDieu-SectEx2','SyntelDieu');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx1','SynGwnD');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx1','EswtGinom');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SectEx1','ExEuthSynt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-MetroDian-SectEx3','MetroDian');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SynParall-SectEx1','SynParall');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx2','EswtGinom');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SectEx3','EswtGinom');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-SynGwnD-SectEx2','SynGwnD');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE1-1','SyntGnAk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntGnAk-SolSE2-1','SyntGnAk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntDian-SolSE1-1','SyntDian');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE1-1','EswtGinom');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-SyntetDian-SyntMes-SolSE1-1','SyntMes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Statistikh-MetThDias-MetraDiasp-MDParat-SectEx1','MDParat');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx1','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE1-1','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE2-1','EswtGinom');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Dian-EswterikoGin-EswtGinom-SolSE3-1','EswtGinom');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-Eutheia-ExisEutheias-ExEuthSynt-SolSE1-1','ExEuthSynt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx1','StoixEll');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE1-1','StoixEll');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SectEx2','StoixEll');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-StoixEll-SolSE2-1','StoixEll');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx2','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE2-1','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx3','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE3-1','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx4','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE4-1','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def2','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Tab1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-Arith-Arithmoi-Def3','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-Elleipsi-ExEfapt-SectEx1','ExEfapt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SectEx1','GenExKykl');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GenExKykl-SolSE1-1','GenExKykl');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-ExisApolTim-SolSE1-1','ExisApolTim');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Analysh-Parag-Paragogos-Tab1','-');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx5','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx1','ExEfKyk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx2','ExEfKyk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx3','ExEfKyk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx4','ExEfKyk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SectEx6','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-GeomEfarm-SectEx1','GeomEfarm');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExKyklou-SolSE5-1','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Geometria-KwnT-ExisKyklou-ExEfKyk-SectEx5','ExEfKyk');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE1-1','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-Parametrikes-SolSE3-1','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-GrammikaSys-MethAnt-SolSE5-1','MethAnt');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Polyonyma-TimhPol-SectEx2','TimhPol');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx1','BasTaut');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx2','BasTaut');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx3','BasTaut');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Tautothtes-BasTaut-SectEx4','BasTaut');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-DiafTetr-SectEx1','DiafTetr');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx1','AthDiafK');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-AlgPar-Paragont-AthDiafK-SectEx2','AthDiafK');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-AnGeom-Conic-Kykl-ExKyklou-SectEx1','ExKyklou');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx1','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SectEx2','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE1-1','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE10-1','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-AorAdynEx-SolSE1-1','AorAdynEx');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE4-1','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Logarithmos-YpolLog-SolSE5-1','YpolLog');
INSERT INTO "ExerciseTypes_per_File" VALUES ('Alg-Exis1ou-EpilExis-SolSE1-1','EpilExis');
INSERT INTO "ExerciseTypes_per_File" VALUES ('DTX-Algebra-ExAn-Exis2ou-Parametrikes-SolSE2-1','Parametrikes');
INSERT INTO "ExerciseTypes_per_File" VALUES ('1','EpilExis');
CREATE TRIGGER Delete_Exercise_Type
AFTER DELETE
ON Sections_Exercises
BEGIN
DELETE FROM Exercise_Types
WHERE Id <> '-' AND Id NOT IN (SELECT DISTINCT Exercise_Id FROM Sections_Exercises);
END;
CREATE TRIGGER Delete_Exercise_Type_on_Update
AFTER UPDATE
ON Sections_Exercises
BEGIN
DELETE FROM Exercise_Types
WHERE Id <> '-' AND Id NOT IN (SELECT DISTINCT Exercise_Id FROM Sections_Exercises);
END;
COMMIT;
