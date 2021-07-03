#include "SDCSV.h"

SDCSVClass SDCSV;

Record* Record::next(){
	return this->nextRec;
}

void Record::setNextRecord(Record *_nextRec){
	this->nextRec = _nextRec;
}

char* Record::getData(uint8_t index){
	if (index>=this->dataCount||index<0) return NULL;
	return this->data[index];
}

char** Record::getData(){
	return this->data;
}

uint8_t Record::getDataCount(){
	return this->dataCount;
}

void Record::addNextVal(const char *val){
	char *stoVal = (char*)malloc(strlen(val)+1);
	strcpy(stoVal, val);
	this->dataCount++;
	this->data = (char**)realloc(this->data, dataCount*sizeof(char*));
	this->data[dataCount-1] = stoVal;
}

void Record::clear(){
	for (uint8_t i = 0;i<this->dataCount;i++){
		free(data[i]);
	}
	free(data);
	this->data = NULL;
	this->dataCount = 0;
}

void Record::close(){
	clear();
}

void Record::remove(uint8_t colIndex){
	if (colIndex>=dataCount || colIndex<0) return;

	char *removedStr = this->data[colIndex];
	this->dataCount--;
	for (uint8_t i = colIndex;i<dataCount;i++){
		this->data[i] = this->data[i+1];
	}

	char** newData = (char**)malloc(dataCount*sizeof(char*));
	memcpy(newData, this->data, dataCount*sizeof(char*));

	free(removedStr);
	free(this->data);

	this->data = newData;
}

void Record::connectToNextNode(Record* _nextRec){
	this->nextRec = _nextRec;
}

///===========================================================///

Record* RecordList::getHead(){
	return this->head;
}

bool RecordList::isEmpty(){
	return (this->head == NULL);
}

void RecordList::close(){
	if (isEmpty()) return;
	Record *rec = this->head;
	Record *nextRec;
	do {
		nextRec = rec->next();
		rec->close();
		free(rec);
		rec = nextRec;
	}while(rec!=NULL);
	this->head = NULL;
}

Record* RecordList::getTail(){
	if (isEmpty()) return NULL;
	Record *tail = this->head;
	while(tail->next()!=NULL){
		tail = tail->next();
	}
	return tail;
}

void RecordList::addNextRecord(Record *rec){
	if (rec == NULL) return;
	Record *tail = getTail();
	tail->connectToNextNode(rec);
	rec->connectToNextNode(NULL);
}

///===========================================================///

void SDCSVClass::addNewRecordToCSV(File *file, const char** strArr, int strAmount){
	Record record;
	for (uint8_t i = 0;i<strAmount;i++){
		record.addNextVal(strArr[i]);
	}
	addNewRecordToCSV(file, &record);
	record.close();
}

void SDCSVClass::addNewRecordToCSV(File *file, Record *rec){
	if (!rec || !file) return;
	file->seek(file->size());
	for (uint8_t i =0;i<rec->getDataCount();i++){
		file->print(rec->getData(i));
		file->print((i<(rec->getDataCount()-1))?',':'\n');
	}
}

RecordList SDCSVClass::getRecordsFromCSV(File *file, int8_t colIndex, const char *colVal){
	RecordList list;
	if (!file) return list;

	file->seek(0);
	Record *rec;
	do {
		rec = getNextRecordInFile(file);
		list.addNextRecord(rec);
	}while(rec!=NULL);

	return list;
}

Record* SDCSVClass::getNextRecordInFile(File *file){
	if (!file || file->position() == file->size()) return NULL;
	Record *rec = (Record*)malloc(sizeof(Record));
	char *rawStr = readln(file);
	char *token = strtok(rawStr, ",");
	while(token!=NULL){
		rec->addNextVal(token);
		token = strtok(NULL, ",");
	}
	return rec;
}

void SDCSVClass::longTo4byte(long val,char a[])
{
  for(int i=0;i<4;++i)
  {
    a[i] = val % (1<<8);
    val /= (1<<8);
  }
}

long SDCSVClass::byteTolong(char a[])
{
  long ans = 0;
  for(int i=0;i<4;++i)
  {
    ans = ans*(1<<8) + a[i]; 
  }
  return ans;
}

char* SDCSVClass::readln(File *myfile, int max)
{
   char tmp;
   long Position = 0;
   char str_tmp[max];
   char* str = NULL;
   do
   {
      myfile->read(&tmp,1);
      if(tmp!='\n')
        str_tmp[Position++] =  tmp;
   }while(tmp!='\n');
   str_tmp[Position++] = '\0';
   
   str = new char[Position];
   for(int i=0;i<Position;++i)
        str[i] = str_tmp[i];
  return str;
}

void SDCSVClass::createNewCSV(const char* fileName, const char** titleCols, int colsAmount){
	SD.remove(fileName);
	File file = SD.open(fileName, FILE_WRITE);
	addNewRecordToCSV(&file, titleCols, colsAmount);
	file.close();
}

char* SDCSVClass::longToString(uint32_t val){
	String str(val);
	char *resStr = (char*)malloc(str.length()+1);
	str.toCharArray(resStr, str.length()+1);
	return resStr;
}

uint32_t SDCSVClass::stringToLong(const char *strN){
	String str(strN);
	return str.toInt();
}
