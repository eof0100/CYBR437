import pytest
from cybr437regex import cybr437regex

#===========================================================

# valid ssn's all must pass
@pytest.mark.parametrize("ssn", [
    "132-45-6789",
    "143 45 6789",
    "135 45-6789",
    "13345-6789",
    "932956789",
])
def test_ssn_pattern_valid_format(ssn):
    assert cybr437regex.ssn_pattern(ssn) == True
    
    
# invalid ssn's all must fail
@pytest.mark.parametrize("ssn", [
    "000-26-6781",
    "111-11-1111",
    "123-45-6789",
    "666-45-6789",
    "932-45-6789",
])    
def test_ssn_pattern_invalid_format(ssn):
    assert cybr437regex.ssn_pattern(ssn) == False

#===========================================================
# All must pass these are valid e-mails that all must pass the test
@pytest.mark.parametrize("email", [
    "j0hn.smith@ewu.edu",
    "John-Smith@ewu.edu",
    "-john_smith-@ewu.edu",
    "a@abcd",
    "artem@ewu.edu",
])
def test_email_pattern_valid_format(email):
    assert cybr437regex.email_pattern(email) == True
    
# all these must FAIL because these are all INVALID 
@pytest.mark.parametrize("email", [
    ".john@ewu.edu",
    "john.smith.@ewu.edu",
    "john..smith@ewu.edu",
    "j.smith@e.u",
    "@ewu.edu",
    ".@ewu.edu"
])
def test_email_pattern_invalid_format(email):
    assert cybr437regex.email_pattern(email) == False

#===========================================================

# Password all valid must pass
@pytest.mark.parametrize("password", [
    "!3tsSeeIf&",
    '@on3Two"hr0',
    "&&&&&&&&sS3",
    "t3sTingM33!",
    "th!$H4$&()",
])
def test_password_pattern_valid_format(password):
    assert cybr437regex.password_pattern(password) == True

@pytest.mark.parametrize("password", [
    "paassWordd",
    "p@sSW()rds",
    "N0LOWERC4S3!",
    "only9ch@R",
    "Pa$sword1212",
])
# Password all invalid all must fail
def test_password_pattern_invalid_format(password):
    assert cybr437regex.password_pattern(password) == False



if __name__ == '__main__':
    pytest.main(["-q", "--tb=no", __file__])
