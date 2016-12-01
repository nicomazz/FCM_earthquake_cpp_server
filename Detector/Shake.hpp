struct Shake{
	User u;
	long millis;
	bool operator<(const Shake & b) const{
		return millis < b.millis;
	}
};