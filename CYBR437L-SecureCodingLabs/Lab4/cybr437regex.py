import re

class cybr437regex:
    
    
    @staticmethod
    def ssn_pattern(ssn_string : str) -> bool:
        if not isinstance(ssn_string, str) or not ssn_string.strip():
            raise ValueError("Bad ssn_string Parameter")
        
        #[0-9]{3} three numbrers 0-9
        #[-s\s]? optional dash or space
        #[0-9]{2} two numbers 0-9
        #[-\s]? optional dash or space
        #[0-9]{4} 4 digits 0-9
        regex = r"^(?!000|666)(?!9[0-9]{2}[-\s]?[0-8])(?!([0-9])\1{2}[-\s]?\1{2}[-\s]?\1{4}$)(?!123[-\s]?45[-\s]?6789$)[0-9]{3}[-\s]?(?!00)[0-9]{2}[-\s]?(?!0000)[0-9]{4}$"  
              
        return re.fullmatch(regex, ssn_string) is not None


    @staticmethod
    def email_pattern(email_string : str) -> bool:
        if not isinstance(email_string, str) or not email_string.strip():
            raise ValueError("Bad email_string Parameter")
            
            
        #
        # ^ beginning of input
        # (?!\.) from the current position (which is the beginning) char cannot be a dot
        # (?!.*\.\.) reject if pattern matches ahead .* zero or more chars followed by  \.\. two dots
        #[A-Za-z0-9_.-] allows any of those one character including the . dot (means period)
        # + one or more preceding chars allowed
        # (?<!\.) character immedsiately before the current position cannot be a dot
        # which is placed before the @ sign so period now allowed before @ sign
        # @ which is the literal @ match seprating the two parts 
        # [A-Za-z0-9_.-]{4,} after the @, any of those chars are allowed 4 or more chars
        # $ says its the end of the input 
        regex = r"^(?!\.)(?!.*\.\.)[A-Za-z0-9_.-]+(?<!\.)@[A-Za-z0-9_.-]{4,}$"
        
        return re.fullmatch(regex, email_string) is not None
    

    @staticmethod
    def date_pattern(date_string : str) -> bool:
        if not isinstance(date_string, str) or not date_string.strip():
            raise ValueError("Bad date_string Parameter")
            
        regex = ""
        
        return re.fullmatch(regex, date_string) is not None
        

    
    @staticmethod
    def password_pattern(password_string : str) -> bool:
        if not isinstance(password_string, str) or not password_string.strip():
            raise ValueError("Bad password_string Parameter")
        
        regex = r"^(?=.{10,}$)(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[!-/:-@\[-`{-~])(?!.*[a-z]{4})(?!.*([0-9]{2}).*\1)[a-z!-/:-@\[-`{-~].*$"
        
        return re.fullmatch(regex, password_string) is not None
        
        
    @staticmethod
    def ion_pattern(ion_string : str) -> bool:
        if not isinstance(ion_string, str) or not ion_string.strip():
            raise ValueError("Bad ion_string Parameter")
        
        regex = ""
        
        return re.fullmatch(regex, ion_string) is not None
    
        
    